#include <openssl/pem.h>

#include "pkey_param_loadwrite.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_PARAM_LOADWRITE_ADDRESS, type, code))

int extractParameters(EVP_PKEY * params, BIO * output)
{
    int iResult;

    iResult = PEM_write_bio_Parameters(output, params);
    if(iResult != 1)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        return -1;
    }

    return 0;
}

EVP_PKEY * loadParameters(EVP_PKEY ** params, BIO * input)
{
    EVP_PKEY * result;

    result = PEM_read_bio_Parameters(input, params);
    if(result == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
    }

    return result;
}
