// gcry.cc
// Copyright (C) 2013  Vedant Kumar <vsk@berkeley.edu>, see ~/LICENSE.txt.

#include "gcry.hh"

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rsa-keypair.sp> <rsa-pubkey.sp>\n", argv[0]);
        xerr("Invalid arguments.");
    }

    gcrypt_init();

    char* fname = argv[1];
    char* keypubnm = argv[2];



     /* Grab a key pair password and create an encryption context with it. */
    gcry_cipher_hd_t aes_hd;
    printf(" Please enter a password to lock \n"
           "your key pair. This password must be committed to memory. \n\n");
    char lockpasswd[4096] = {0,};
    get_aes_ctx(&aes_hd, lockpasswd);

    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;
//int p_sizes[] = { 1024, 2048, 3072, 4096, 8192, 16384, 32768 };
    err = gcry_sexp_build(&rsa_parms, NULL, "(genkey (rsa (nbits 4:8192)))");
    if (err) {
        gcry_sexp_release(rsa_parms);
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to create rsa params");
    }

     /* Generate a new RSA key pair. */
    printf("RSA key generation can take a few minutes. Your computer \n"
           "needs to gather random entropy. So Move the mouse, or use the keyboard to write something\n"
         "and wait... \n\n");
    err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
    gcry_sexp_release(rsa_parms);
    if (err) {
        gcry_sexp_release(rsa_keypair);
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to create rsa key pair");
    }

    gcry_sexp_t pubk = gcry_sexp_find_token(rsa_keypair, "public-key", 0);
    //gcry_sexp_t privk = gcry_sexp_find_token(rsa_keypair, "private-key", 0);

    int pubkey_len = gcry_sexp_sprint (pubk, GCRYSEXP_FMT_CANON, 0, 0);
    void *pubkey_buf = malloc(pubkey_len);
    gcry_sexp_sprint (pubk, GCRYSEXP_FMT_CANON, pubkey_buf, pubkey_len);
    gcry_sexp_release(pubk);
    int wsize = write_tofile(keypubnm, pubkey_buf, pubkey_len);
    free(pubkey_buf);
    if(wsize != pubkey_len) printf("write public key file failed\n");
    printf("RSA key generation complete!\n");



    /* Encrypt the RSA key pair. */
    //size_t rsa_len = get_keypair_size(2048);
    size_t keypair_canon_len = gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, 0, 0);
    size_t rsa_len = keypair_canon_len; //get_keypair_size(2048);
    void* rsa_buf = calloc(1, keypair_canon_len + 1 /*rsa_len*/);
    if (!rsa_buf) {
        xerr("malloc: could not allocate rsa buffer");
    }
    gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, rsa_buf, keypair_canon_len /*rsa_len*/);
    gcry_sexp_release(rsa_keypair);
    printf("alloc rsa key buffer:%d, cannon len:%d\n", (int)rsa_len, (int)keypair_canon_len);
    //printf("%s\n", (char*)rsa_buf);

    //rsa_len = keypair_canon_len;

    err = gcry_cipher_encrypt(aes_hd, (unsigned char*) rsa_buf,
                              rsa_len, NULL, 0);
    gcry_cipher_close(aes_hd);
    if (err) {
        free(rsa_buf);
        xerr("gcrypt: could not encrypt with AES");
    }

    FILE* lockf = fopen(fname, "wb");
    if (!lockf) {
        free(rsa_buf);
        xerr("fopen() failed");
    }

    /* Write the encrypted key pair to disk. */
    if (fwrite(rsa_buf, rsa_len, 1, lockf) != 1) {
        perror("fwrite");
        free(rsa_buf);
        fclose(lockf);
        xerr("fwrite() failed");
    }

    free(rsa_buf);
    /* Release contexts. */
    //gcry_sexp_release(rsa_keypair);
    //gcry_sexp_release(rsa_parms);
    //gcry_cipher_close(aes_hd);

    fclose(lockf);

    printf("create finish\n Please input your password again\n");
    int cnts = 0;
    do{
        char *chkwd = get_pass_str();
        if(chkwd && !strcmp(chkwd, lockpasswd)){
            break;
        }
        printf("password mismatch, Please input again\n");
    }while(++cnts < 3);

    if(cnts == 3){
        printf("sorry, you lost password.\n");
    }else{
        printf("password ok\n private key is: %s\n public key is:%s\n", fname, keypubnm);
    }

    return 0;
}
