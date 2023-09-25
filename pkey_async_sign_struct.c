#include <string.h>

#include "pkey_async_sign_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_SIGN_STRUCT_ADDRESS, type, code))

struct async_sign_data * async_sign_data_new()
{
    struct async_sign_data * data = NULL;

    data = (struct async_sign_data*)malloc(sizeof(struct async_sign_data));
    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    data->inputPath = NULL;
    data->outputPath = NULL;
    data->keyPath = NULL;

    data->passphrase = NULL;

    data->handler = NULL;

    data->digest = 0;
    data->cipher = 0;

    data->flags = 0;

end:
    return data;
};

int async_sign_data_delete(struct async_sign_data * data)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    data->inputPath = NULL;
    data->outputPath = NULL;
    data->keyPath = NULL;

    data->passphrase = NULL;

    data->handler = NULL;

    data->digest = 0;
    data->cipher = 0;

    data->flags = 0;

    free(data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_sign_data_provide(struct async_sign_data * data, const char * inputPath, const char * outputPath, const char * keyPath,
                            const char * passphrase, struct asynchronicHandler * handler,
                            int digest, int cipher, unsigned char flags)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    data->inputPath = inputPath;
    data->outputPath = outputPath;
    data->keyPath = keyPath;

    data->passphrase = passphrase;

    data->handler = handler;

    data->digest = digest;
    data->cipher = cipher;

    data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
