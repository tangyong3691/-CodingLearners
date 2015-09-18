// gcry.hh
// Copyright (C) 2013  Vedant Kumar <vsk@berkeley.edu>, see ~/LICENSE.txt.

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <gcrypt.h>

/* Crash routine. */
void xerr(const char* msg);

/* Initialize libgcrypt. */
void gcrypt_init();

/* Estimate the size of the encrypted key pair. */
/*size_t get_keypair_size(int nbits);*/

/* Create an AES context out of a user's password. */
void get_aes_ctx(gcry_cipher_hd_t* aes_hd, char *buf);


int write_tofile(const char *filenm, void *buf, int len);

int read_fromfile(const char *filenm, void *buf, int buf_size);

int get_filesize(const char *filenm);

struct crypt_fileinfo{
    char keyfile[1024];
    char inputfile[1024];
    char outputfile[1024];
};

int parse_cmdline_arg(struct crypt_fileinfo *inf, int argc, char *argv[]);

int init_crypt_aes(gcry_cipher_hd_t* aes_hd, const char *passwd);

char *get_pass_str(void);
