// main.cc
// Copyright (C) 2013  Vedant Kumar <vsk@berkeley.edu>, see ~/LICENSE.txt.

#include "gcry.hh"

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <rsa-keypair.sp>\n", argv[0]);
        xerr("Invalid arguments.");
    }

    gcrypt_init();
    gcry_error_t err;

    char* fname = argv[1];
    FILE* lockf = fopen(fname, "rb");
    if (!lockf) {
        xerr("fopen() failed");
    }

    /* Grab a key pair password and create an AES context with it. */
    gcry_cipher_hd_t aes_hd;
    get_aes_ctx(&aes_hd, 0);

    /* Read and decrypt the key pair from disk. */
    fseek(lockf, 0, SEEK_END);

    size_t rsa_len = ftell(lockf);/*get_keypair_size(2048);*/
    fseek(lockf, 0, SEEK_SET);
    void* rsa_buf = calloc(1, rsa_len);
    if (!rsa_buf) {
        xerr("malloc: could not allocate rsa buffer");
    }

    if (fread(rsa_buf, rsa_len, 1, lockf) != 1) {
        xerr("fread() failed");
    }

    fclose(lockf);

    err = gcry_cipher_decrypt(aes_hd, (unsigned char*) rsa_buf,
                              rsa_len, NULL, 0);
    if (err) {
        xerr("gcrypt: failed to decrypt key pair");
    }

    /* Load the key pair components into sexps. */
    gcry_sexp_t rsa_keypair;
    err = gcry_sexp_new(&rsa_keypair, rsa_buf, rsa_len, 0);
    gcry_sexp_t pubk = gcry_sexp_find_token(rsa_keypair, "public-key", 0);
    gcry_sexp_t privk = gcry_sexp_find_token(rsa_keypair, "private-key", 0);

    int pubkey_len = gcry_sexp_sprint (pubk, GCRYSEXP_FMT_CANON, 0, 0);
    void *pubkey_buf = malloc(pubkey_len);
    gcry_sexp_sprint (pubk, GCRYSEXP_FMT_CANON, pubkey_buf, pubkey_len);
    write_tofile("./pub_key_file1", pubkey_buf, pubkey_len);
    free(pubkey_buf);
    /* Create a message. */
    gcry_mpi_t msg;
    gcry_sexp_t data;
    const unsigned char* s = (const unsigned char*) "Hello world.";
    err = gcry_mpi_scan(&msg, GCRYMPI_FMT_USG, s,
                        strlen((const char*) s), NULL);
    if (err) {
        xerr("failed to create a mpi from the message");
    }

    err = gcry_sexp_build(&data, NULL,
                           "(data (flags raw) (value %m))", msg);
    if (err) {
        xerr("failed to create a sexp from the message");
    }

    /* Encrypt the message. */
    gcry_sexp_t ciph;
    err = gcry_pk_encrypt(&ciph, data, pubk);
    if (err) {
        xerr("gcrypt: encryption failed");
    }

    int cry_data_sz = gcry_sexp_sprint (ciph, GCRYSEXP_FMT_CANON, 0, 0);
    void *cry_data_buf = malloc(cry_data_sz);
    gcry_sexp_sprint (ciph, GCRYSEXP_FMT_CANON, cry_data_buf, cry_data_sz);
    write_tofile("./out_crypted_example", cry_data_buf, cry_data_sz);
    gcry_sexp_t ciph2;
    gcry_sexp_new (&ciph2, cry_data_buf, cry_data_sz, 0);
    free(cry_data_buf);
    /*gcry_mpi_t crypt_msg = gcry_sexp_nth_mpi(ciph, 0, GCRYMPI_FMT_USG);
    printf("crypt_msg:\n");
    gcry_mpi_dump(crypt_msg);
    printf("\n");*/

    /* Decrypt the message. */
    gcry_sexp_t plain;
    err = gcry_pk_decrypt(&plain, ciph2, privk);
    if (err) {
        xerr("gcrypt: decryption failed");
    }

    /* Pretty-print the results. */
    gcry_mpi_t out_msg = gcry_sexp_nth_mpi(plain, 0, GCRYMPI_FMT_USG);
    printf("Original:\n");
    gcry_mpi_dump(msg);
    printf("\n" "Decrypted:\n");
    gcry_mpi_dump(out_msg);
    printf("\n");

    if (gcry_mpi_cmp(msg, out_msg)) {
        xerr("data corruption!");
    }

    printf("Messages match.\n");

    unsigned char obuf[64] = { 0 };
    err = gcry_mpi_print(GCRYMPI_FMT_USG, (unsigned char*) obuf,
                         sizeof(obuf), NULL, out_msg);
    if (err) {
        xerr("failed to stringify mpi");
    }
    printf("-> %s\n", (char*) obuf);


    /* Release contexts. */
    gcry_mpi_release(msg);
    gcry_mpi_release(out_msg);
    gcry_sexp_release(rsa_keypair);
    gcry_sexp_release(pubk);
    gcry_sexp_release(privk);
    gcry_sexp_release(data);
    gcry_sexp_release(ciph);
    gcry_sexp_release(ciph2);
    gcry_sexp_release(plain);
    gcry_cipher_close(aes_hd);
    free(rsa_buf);


    return 0;
}
