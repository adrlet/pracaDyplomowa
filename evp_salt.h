#ifndef EVP_SALT_H_INCLUDED
#define EVP_SALT_H_INCLUDED

#include <openssl/bio.h>

#define ERROR_SALT_ADDRESS ERROR_FUNCTION_ADDRESS(11)

#define SALT_HEADER_LEN 8
#define DEFAULT_SALT_LEN 8

int writeSalt(BIO * output, const unsigned char * salt, int saltlen, unsigned char flush);

int readSalt(BIO * input, unsigned char * salt, int saltlen);

#endif // EVP_SALT_H_INCLUDED
