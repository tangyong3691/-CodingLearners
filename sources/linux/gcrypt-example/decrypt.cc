// decrypt.cc
// Copyright (C) 2015  Tang Yong, see ~/LICENSE.txt.

#include "gcry.hh"

//Using: ./decrypt -k <keyfile> <inputfile> <outputfile>
int main(int argc, char** argv)
{
    struct crypt_fileinfo finf;

    if(parse_cmdline_arg(&finf, argc, argv)){
        printf("Using: ./decrypt -k <keyfile> <inputfile> <outputfile>\n");
        exit(1);
    }
    int keylen;
    void *keybuf = malloc(keylen = get_filesize(finf.keyfile));
    if(keylen != read_fromfile(finf.keyfile, keybuf, keylen)){
        printf("read from key file failed.\n");
        free(keybuf);
        exit(1);
    }

    gcry_cipher_hd_t aes_hd;
    gcry_error_t err;

    //get_aes_ctx(&aes_hd);
    char* passwd = get_pass_str();
    if( (!passwd) || init_crypt_aes(&aes_hd, passwd)){
        printf("init aes crypt failed.\n");
        free(keybuf);
        exit(1);
    }

    err = gcry_cipher_decrypt(aes_hd, (unsigned char*) keybuf,
                              keylen, NULL, 0);

    if (err) {
        free(keybuf);
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to decrypt key pair");
    }

    gcry_sexp_t rsa_keypair;
    err = gcry_sexp_new(&rsa_keypair, keybuf, keylen, 0);
    free(keybuf);
    if(err){
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to parse cannon format\n");
    }
    gcry_sexp_t pubk = gcry_sexp_find_token(rsa_keypair, "public-key", 0);
    gcry_sexp_t privk = gcry_sexp_find_token(rsa_keypair, "private-key", 0);
    if( (!pubk) || (!privk)){
        gcry_sexp_release(rsa_keypair);
        gcry_sexp_release(pubk);
        gcry_sexp_release(privk);
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to parse key\n");
    }
    gcry_sexp_release(pubk);
    gcry_sexp_release(rsa_keypair);

    int in_data_size = get_filesize(finf.inputfile);
    unsigned char* in_data = (unsigned char*)malloc(in_data_size);
    if(in_data_size != read_fromfile(finf.inputfile, in_data, in_data_size)){
        printf("read data from crypt file failed.\n");
        free(in_data);
        gcry_sexp_release(privk);
        gcry_cipher_close(aes_hd);
        exit(1);
    }

    gcry_sexp_t ciph;

    gcry_sexp_new (&ciph, in_data, in_data_size, 0);
    free(in_data);
    if(!ciph){
        gcry_sexp_release(privk);
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: failed to parse crypt data cannon format\n");
    }

    /* Decrypt the message. */
    gcry_sexp_t plain;
    err = gcry_pk_decrypt(&plain, ciph, privk);
    gcry_sexp_release(privk);
    gcry_sexp_release(ciph);
    if (err) {
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: decryption failed");
    }

    /* Pretty-print the results. */
    gcry_mpi_t out_msg = gcry_sexp_nth_mpi(plain, 0, GCRYMPI_FMT_USG);
    gcry_sexp_release(plain);
    if(!out_msg){
        gcry_cipher_close(aes_hd);
        xerr("gcrypt: decrypt text fmt usg parse failed\n");
    }
    /*printf("Original:\n");
    gcry_mpi_dump(msg);
    printf("\n" "Decrypted:\n");
    gcry_mpi_dump(out_msg);
    printf("\n");

    if (gcry_mpi_cmp(msg, out_msg)) {
        xerr("data corruption!");
    }

    printf("Messages match.\n");*/

    int out_size = 0;
    gcry_mpi_print(GCRYMPI_FMT_USG, 0, 0, (size_t*)&out_size, out_msg);
    printf("out size:%d\n", out_size);
    //sleep(2);
    void *out_buf = malloc(out_size);

    err = gcry_mpi_print(GCRYMPI_FMT_USG, (unsigned char*) out_buf,
                         out_size, NULL, out_msg);
    if (err) {
        free(out_buf);
        gcry_mpi_release(out_msg);
        gcry_cipher_close(aes_hd);
        xerr("failed to stringify mpi");
    }

    write_tofile(finf.outputfile, out_buf, out_size);
    free(out_buf);

    if(out_size != get_filesize(finf.outputfile)){
        printf("write decrypted file failed.\n");
    }

    //gcry_mpi_release(msg);
    gcry_mpi_release(out_msg);
    //gcry_sexp_release(rsa_keypair);
    //gcry_sexp_release(pubk);
    //gcry_sexp_release(privk);
    //gcry_sexp_release(ciph);
    //gcry_sexp_release(plain);
    gcry_cipher_close(aes_hd);

    printf("decrypt ok\n");
}
