#ifndef EVP_ENCRYPTDECRYPT_H_INCLUDED
#define EVP_ENCRYPTDECRYPT_H_INCLUDED

#include <openssl/evp.h>
#include <openssl/bio.h>

#define ERROR_ENCRYPT_DECRYPT_ADDRESS ERROR_FUNCTION_ADDRESS(5)

struct cipherData;
struct asynchronicHandler;

int encrypt(BIO * input, BIO * output, struct cipherData * cipherParameters, struct asynchronicHandler * handler, ENGINE * engine);

int decrypt(BIO * input, BIO * output, struct cipherData * cipherParameters, struct asynchronicHandler * handler, ENGINE * engine);

#endif // EVP_ENCRYPTDECRYPT_H_INCLUDED
