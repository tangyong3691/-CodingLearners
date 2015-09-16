#include "gcry.hh"

void xerr(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void write_tofile(char *filenm, void *buf, int len)
{
     FILE * f = fopen(filenm, "w");
     fwrite(buf, len, 1, f);
     fclose(f);
}

void gcrypt_init()
{
    printf("gcrypt ver:%s\n", gcry_check_version (0));
    /* Version check should be the very first call because it
       makes sure that important subsystems are intialized. */
    if (!gcry_check_version (GCRYPT_VERSION))
    {
        xerr("gcrypt: library version mismatch");
    }

    gcry_error_t err = 0;

    /* We don't want to see any warnings, e.g. because we have not yet
       parsed program options which might be used to suppress such
       warnings. */
    err = gcry_control (GCRYCTL_SUSPEND_SECMEM_WARN);

    /* ... If required, other initialization goes here.  Note that the
       process might still be running with increased privileges and that
       the secure memory has not been intialized.  */

    /* Allocate a pool of 16k secure memory.  This make the secure memory
       available and also drops privileges where needed.  */
    err |= gcry_control (GCRYCTL_INIT_SECMEM, 16384, 0);

    /* It is now okay to let Libgcrypt complain when there was/is
       a problem with the secure memory. */
    err |= gcry_control (GCRYCTL_RESUME_SECMEM_WARN);

    /* ... If required, other initialization goes here.  */

    /* Tell Libgcrypt that initialization has completed. */
    err |= gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);

    if (err) {
        xerr("gcrypt: failed initialization");
    }
}

size_t get_keypair_size(int nbits)
{
    size_t aes_blklen = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES128);

    // format overhead * {pub,priv}key (2 * bits)
    size_t keypair_nbits = 4 * (2 * nbits);

    size_t rem = keypair_nbits % aes_blklen;
    return (keypair_nbits + rem) / 8;
}

void get_aes_ctx(gcry_cipher_hd_t* aes_hd)
{
    const size_t keylen = 16;
    char passwd_hash[keylen];

    char* passwd = getpass("Keypair Password: ");
    size_t pass_len = passwd ? strlen(passwd) : 0;
    if (pass_len == 0) {
        xerr("getpass: not a valid password");
    }

    int err = gcry_cipher_open(aes_hd, GCRY_CIPHER_AES128,
                               GCRY_CIPHER_MODE_CFB, 0);
    if (err) {
        xerr("gcrypt: failed to create aes handle");
    }

    memset(passwd_hash, 0, sizeof(passwd_hash));
    gcry_md_hash_buffer(gcry_md_map_name("MD5"),/*GCRY_MD_MD5,*/ (void*) &passwd_hash,
                        (const void*) passwd, pass_len);
    int ii;
    printf("pass:%s\nlen:%d , md5:\n", passwd, pass_len);
    for(ii=0; ii < 16; ii++) printf("%02x", (unsigned char)passwd_hash[ii]);
    printf("\n");

    memset(passwd_hash, 0, sizeof(passwd_hash));
    gcry_md_hd_t h;
    int algo = gcry_md_map_name("SHA256");
     gcry_md_open (&h, algo, 0 /*, GCRY_MD_FLAG_HMAC 0*/);
    gcry_md_write (h, (void *) passwd, pass_len);
      gcry_md_final (h);
      char passwd_hash256[keylen*2];
      memcpy (passwd_hash256, gcry_md_read (h, algo), gcry_md_get_algo_dlen (algo));
      gcry_md_close (h);
    for(ii=0; ii < 32; ii++) printf("%02x", (unsigned char)passwd_hash256[ii]);
    printf("\n");

    err = gcry_cipher_setkey(*aes_hd, (const void*) &passwd_hash, keylen);
    if (err) {
        xerr("gcrypt: could not set cipher key");
    }

    err = gcry_cipher_setiv(*aes_hd, (const void*) &passwd_hash, keylen);
    if (err) {
        xerr("gcrypt: could not set cipher initialization vector");
    }
}
