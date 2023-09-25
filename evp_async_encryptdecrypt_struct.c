#include <stddef.h>

#include "evp_async_encryptdecrypt_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_ENCRYPT_DECRYPT_STRUCT_ADDRESS, type, code))

struct async_encrypt_data * async_encrypt_data_new()
{
    struct async_encrypt_data * encrypt_data = NULL;

    encrypt_data = (struct async_encrypt_data*)malloc(sizeof(struct async_encrypt_data));
    if(encrypt_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    encrypt_data->inputPath = NULL;
    encrypt_data->outputPath = NULL;

    encrypt_data->key = NULL;
    encrypt_data->iv = NULL;

    encrypt_data->engine = NULL;

    encrypt_data->handler = NULL;

    encrypt_data->cipher_algorithm_position = 0;
    encrypt_data->saltlen = 0;

    encrypt_data->flags = 0;

end:
    return encrypt_data;
};

int async_encrypt_data_delete(struct async_encrypt_data * encrypt_data)
{
    int iResult = 0;

    if(encrypt_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    encrypt_data->inputPath = NULL;
    encrypt_data->outputPath = NULL;

    encrypt_data->key = NULL;
    encrypt_data->iv = NULL;

    encrypt_data->engine = NULL;

    encrypt_data->handler = NULL;

    encrypt_data->cipher_algorithm_position = 0;
    encrypt_data->saltlen = 0;

    encrypt_data->flags = 0;

    free(encrypt_data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_encrypt_data_provide(struct async_encrypt_data * encrypt_data, const char * inputPath, const char * outputPath,
                               unsigned char * key, unsigned char * iv, ENGINE * engine, struct asynchronicHandler * handler,
                               int cipher_algorithm_position, int saltlen, unsigned char flags)
{
    int iResult = 0;

    if(encrypt_data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    encrypt_data->inputPath = inputPath;
    encrypt_data->outputPath = outputPath;

    encrypt_data->key = key;
    encrypt_data->iv = iv;

    encrypt_data->engine = engine;

    encrypt_data->handler = handler;

    encrypt_data->cipher_algorithm_position = cipher_algorithm_position;
    encrypt_data->saltlen = saltlen;

    encrypt_data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
