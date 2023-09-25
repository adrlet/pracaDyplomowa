#include <openssl/ec.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>

#include "pkey_paramgen.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_PARAM_GEN_ADDRESS, type, code))

/// Wrapping macros in functions to cast them into pointers
int EVP_PKEY_CTX_set_ec_paramgen_curve_nid_func(EVP_PKEY_CTX * ctx, int bits)
{
    return EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, bits);
}

int EVP_PKEY_CTX_set_dsa_paramgen_bits_func(EVP_PKEY_CTX * ctx, int bits)
{
    return EVP_PKEY_CTX_set_dsa_paramgen_bits(ctx, bits);
}

int EVP_PKEY_CTX_set_dh_paramgen_prime_len_func(EVP_PKEY_CTX * ctx, int bits)
{
    return EVP_PKEY_CTX_set_dh_paramgen_prime_len(ctx, bits);
}

int generateParameters(int type, int count, int bit, EVP_PKEY ** parameters, ENGINE * engine)
{
    EVP_PKEY_CTX * ctx = NULL;
    int iResult;
    int i = 0;
    int (*parameter_size_func)(EVP_PKEY_CTX *, int);

    if(count < 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto generateParameters_end;
    }

    if(bit < 0)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto generateParameters_end;
    }

    if(parameters == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto generateParameters_end;
    }

    switch(type)
    {
    case EVP_PKEY_EC:
        parameter_size_func = EVP_PKEY_CTX_set_ec_paramgen_curve_nid_func;
        bit = NID_X9_62_prime256v1;
        break;
    case EVP_PKEY_DSA:
        parameter_size_func = EVP_PKEY_CTX_set_dsa_paramgen_bits_func;
        break;
    case EVP_PKEY_DH:
        parameter_size_func = EVP_PKEY_CTX_set_dh_paramgen_prime_len_func;
        break;
    default:
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto generateParameters_end;
    }

    ctx = EVP_PKEY_CTX_new_id(type, engine);
    if(ctx == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_OPENSSL, 0);
        goto generateParameters_end;
    }

    for (i = 0; i < count; i++)
    {
        iResult = EVP_PKEY_paramgen_init(ctx);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 1);
            goto generateParameters_end;
        }
        iResult = parameter_size_func(ctx, bit);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 2);
            goto generateParameters_end;
        }

        parameters[i] = NULL;
        iResult = EVP_PKEY_paramgen(ctx, parameters + i);
        if(iResult != 1)
        {
            writeErrorCode_full(ERROR_TYPE_OPENSSL, 3);
            goto generateParameters_end;
        }
    }

generateParameters_end:
    if(ctx == NULL)
        EVP_PKEY_CTX_free(ctx);
    return i;
}
