#include <openssl/pem.h>

#include "pkey_pub_loadwrite.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_PUB_LOADWRITE_ADDRESS, type, code))

// PKCS#1 RSAPublicKey structure
int extractRsaPublicKey(EVP_PKEY * pub_key, BIO * output)
{
    int iResult;

    iResult = PEM_write_bio_PUBKEY(output, pub_key);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        return -1;
    }

    return 0;
}

// encoded as a SubjectPublicKeyInfo structure
EVP_PKEY * importRsaPublicKey(BIO * input, EVP_PKEY ** pub_key)
{
    EVP_PKEY * iResult;

    iResult = PEM_read_bio_PUBKEY(input, pub_key, NULL, NULL);
    if(iResult == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
    }

    return iResult;
}
