#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_async_param.h"
#include "pkey_async_param_struct.h"

#include "pkey_check.h"
#include "pkey_keygen.h"
#include "pkey_paramgen.h"
#include "pkey_param_loadwrite.h"

#include "errorLog.h"
#include "bio_pre.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_PARAM_GEN_ADDRESS, type, code))

void * async_param_gen(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    printf("async_param_gen\n");

    int iResult = 0;
    int * iResult_alloc = NULL;
    int type = 0;

    struct async_param_data * data = (struct async_param_data*)arg;

    BIO * paramsIO = NULL;

    EVP_PKEY ** param = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    param = (EVP_PKEY**)malloc(sizeof(EVP_PKEY*));
    *param = NULL;

    paramsIO = writeFile(data->paramPath);
    if(paramsIO == NULL)
        goto error;

    switch(data->algorithm)
    {
    case 0:
        type = EVP_PKEY_EC;
        break;
    case 1:
        type = EVP_PKEY_DSA;
        break;
    case 2:
        type = EVP_PKEY_DH;
        break;
    }

    iResult = generateParameters(type, 1, data->keySize, param, NULL);
    if(iResult != 1)
        goto error;
    iResult = extractParameters(*param, paramsIO);
    if(iResult != 0)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(paramsIO != NULL)
    {
        BIO_flush(paramsIO);
        freeBio(paramsIO);
    }
    if(param != NULL)
    {
        if(*param != NULL)
            EVP_PKEY_free(*param);
        free(param);
    }

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}

void * async_param_check(void * arg)
{
    if(arg == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    printf("async_param_check\n");

    int iResult = 0;
    int * iResult_alloc = NULL;

    struct async_param_data * data = (struct async_param_data*)arg;

    BIO * paramsIO = NULL;

    EVP_PKEY * param = NULL;

    iResult_alloc = (int*)malloc(sizeof(int));

    paramsIO = readFile(data->paramPath);
    if(paramsIO == NULL)
        goto error;

    param = loadParameters(NULL, paramsIO);
    if(param == NULL)
        goto error;

    iResult = checkParams(param, NULL);
    if(iResult != 0)
        goto error;

    iResult = 1;
    goto end;

error:
    iResult = error_log_async_write();

end:
    if(paramsIO != NULL)
        freeBio(paramsIO);
    if(param != NULL)
        EVP_PKEY_free(param);

    if(iResult_alloc != NULL)
        iResult_alloc[0] = iResult;

    return iResult_alloc;
}
