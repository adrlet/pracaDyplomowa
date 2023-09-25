#include <openssl/pem.h>

#include "pkey_priv_loadwrite.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_PRIV_LOADWRITE_ADDRESS, type, code))

// use PKCS#8 private key format using PKCS#5 v2.0 password based encryption algorithms
int extractRsaPrivateKey(EVP_PKEY * priv_key, BIO * output, struct privPassword * provider)
{
    int iResult;

    iResult = PEM_write_bio_PKCS8PrivateKey(output, priv_key, provider->algorithm, NULL, 0, provider->pass_provider, provider->phrase);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        return -1;
    }

    return 0;
}

EVP_PKEY * importRsaPrivateKey(BIO * input, EVP_PKEY ** priv_key, struct privPassword * provider)
{
    EVP_PKEY * iResult;

    iResult = PEM_read_bio_PrivateKey(input, priv_key, provider->pass_provider, provider->phrase);
    if(iResult == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
    }

    return iResult;
}
