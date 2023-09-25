#ifndef EVP_ENCRYPTDECRYPT_STRUCT_H_INCLUDED
#define EVP_ENCRYPTDECRYPT_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_ENCRYPT_DECRYPT_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(6)

struct cipherData
{
    unsigned char * key; // 8
    unsigned char * iv; // 16
    const EVP_CIPHER * algorithm; // 24

    int key_len; // 28
    int iv_len; // 32
};

struct cipherData * setupCipher();

void freeCipher(struct cipherData * cipherParameters);

int provideCipherAlgorithm(struct cipherData * cipherParameters, const EVP_CIPHER * algorithm);

int provideCipherKeyIv(struct cipherData * cipherParameters, unsigned char * key, unsigned char * iv);

#endif // EVP_ENCRYPTDECRYPT_STRUCT_H_INCLUDED
