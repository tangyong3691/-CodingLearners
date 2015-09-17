// encrypt.cc
// Copyright (C) 2015  Tang Yong, see ~/LICENSE.txt.

#include "gcry.hh"

//Using: ./encrypt -k <publickeyfile> <inputfile> <outputfile>

int main(int argc, char** argv)
{
    struct crypt_fileinfo finf;
    if(parse_cmdline_arg(&finf, argc, argv)){
        printf("Using: ./encrypt -k <publickeyfile> <inputfile> <outputfile>\n");
        exit(1);
    }

    int keylen;
    void *keybuf = malloc(keylen = get_filesize(finf.keyfile));
    if(keylen != read_fromfile(finf.keyfile, keybuf, keylen)){
        printf("read from key file failed.\n");
        if(keybuf){
            memset(keybuf, 0, keylen);
            free(keybuf);
        }
        exit(1);
    }

    gcry_sexp_t rsa_pbkey;
    gcry_error_t err;
    err = gcry_sexp_new(&rsa_pbkey, keybuf, keylen, 0);
    memset(keybuf, 0, keylen);
    free(keybuf);
    if (err) {
        gcry_sexp_release(rsa_pbkey);
        xerr("gcrypt: public key create failed.\n");
    }

    int in_data_size = get_filesize(finf.inputfile);
    unsigned char* in_data = (unsigned char*)malloc(in_data_size);
    if(in_data_size != read_fromfile(finf.inputfile, in_data, in_data_size)){
        printf("read data from crypt file failed.\n");
        memset(in_data, 0, in_data_size);
        free(in_data);
        gcry_sexp_release(rsa_pbkey);
        exit(1);
    }

    gcry_mpi_t msg;
    gcry_sexp_t data;

    err = gcry_mpi_scan(&msg, GCRYMPI_FMT_USG, in_data,
                        in_data_size, NULL);
    memset(in_data, 0, in_data_size);
    free(in_data);
    if (err) {
        gcry_sexp_release(rsa_pbkey);
        gcry_mpi_release(msg);
        xerr("failed to create a mpi from the message");
    }

    err = gcry_sexp_build(&data, NULL,
                           "(data (flags raw) (value %m))", msg);
    gcry_mpi_release(msg);
    if (err) {
        gcry_sexp_release(rsa_pbkey);
        gcry_sexp_release(data);
        xerr("failed to create a sexp from the message");
    }

    /* Encrypt the message. */
    gcry_sexp_t ciph;
    err = gcry_pk_encrypt(&ciph, data, rsa_pbkey);
    gcry_sexp_release(rsa_pbkey);
    gcry_sexp_release(data);
    if (err) {
        gcry_sexp_release(ciph);
        xerr("gcrypt: encryption failed");
    }

    /*gcry_mpi_t crypted_msg = gcry_sexp_nth_mpi(ciph, 0, GCRYMPI_FMT_USG);
    gcry_sexp_release(ciph);
    if(!crypted_msg){
        xerr("gcrypt: convert crypted mpi failed");
    }

    int cry_data_sz = 0;
    gcry_mpi_print(GCRYMPI_FMT_USG, 0, 0, (size_t *)&cry_data_sz, crypted_msg);
    if(!cry_data_sz){
        gcry_mpi_release(crypted_msg);
        xerr("gcrypt: mpi out failed");
    }*/


    int cry_data_sz = gcry_sexp_sprint (ciph, GCRYSEXP_FMT_CANON, 0, 0);
    if(!cry_data_sz){
        gcry_sexp_release(ciph);
        xerr("gcrypt: encryption get canno size failed.\n");
    }
    void *cry_data_buf = malloc(cry_data_sz);
    int size_cnnon_dt = gcry_sexp_sprint (ciph, GCRYSEXP_FMT_CANON, cry_data_buf, cry_data_sz);
    gcry_sexp_release(ciph);

    //err = gcry_mpi_print(GCRYMPI_FMT_USG, (unsigned char *)cry_data_buf, cry_data_sz, 0, crypted_msg);
    //gcry_mpi_release(crypted_msg);
    if (size_cnnon_dt != cry_data_sz) {
        memset(cry_data_buf, 0, cry_data_sz);
        free(cry_data_buf);
        xerr("failed to stringify mpi");
    }

    write_tofile(finf.outputfile, cry_data_buf, cry_data_sz);
    memset(cry_data_buf, 0, cry_data_sz);
    free(cry_data_buf);
    if(cry_data_sz != get_filesize(finf.outputfile)){
        printf("out secret file failed\n");
    }
    printf("encrypt ok\n");
    return 0;
}
