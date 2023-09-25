#ifndef EVP_ASYNC_ENCRYPTDECRYPT_STRUCT_H_INCLUDED
#define EVP_ASYNC_ENCRYPTDECRYPT_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_ASYNC_ENCRYPT_DECRYPT_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(6)

struct async_encrypt_data
{
    const char * inputPath; // 8
    const char * outputPath; // 16

    unsigned char * key; // 24
    unsigned char * iv; // 32

    ENGINE * engine; // 40

    struct asynchronicHandler * handler; // 48

    int cipher_algorithm_position; // 52
    int saltlen; // 56

    unsigned char flags; // 57
};

struct async_encrypt_data * async_encrypt_data_new();

int async_encrypt_data_delete(struct async_encrypt_data * encrypt_data);

int async_encrypt_data_provide(struct async_encrypt_data * encrypt_data, const char * inputPath, const char * outputPath,
                               unsigned char * key, unsigned char * iv, ENGINE * engine, struct asynchronicHandler * handler,
                               int cipher_algorithm_position, int saltlen, unsigned char flags);

#endif // EVP_ASYNC_ENCRYPTDECRYPT_STRUCT_H_INCLUDED
