#include <string.h>

#include "pkey_async_agremeent_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_AGREMEENT_STRUCT_ADDRESS, type, code))

struct async_agremeent_data * async_agremeent_data_new()
{
    struct async_agremeent_data * data = NULL;

    data = (struct async_agremeent_data*)malloc(sizeof(struct async_agremeent_data));
    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    data->keyPath = NULL;
    data->privKeyPath = NULL;
    data->pubKeyPath = NULL;
    data->outputPath = NULL;
    data->cipher = 0;

    data->flags = 0;

end:
    return data;
};

int async_agremeent_data_delete(struct async_agremeent_data * data)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    data->keyPath = NULL;
    data->privKeyPath = NULL;
    data->pubKeyPath = NULL;
    data->outputPath = NULL;
    data->passphrase = NULL;

    data->cipher = 0;

    data->flags = 0;

    free(data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_agremeent_data_provide(struct async_agremeent_data * data, const char * keyPath, const char * privKeyPath, const char * pubKeyPath,
                                 const char * outputPath, const char * passphrase, int cipher, unsigned char flags)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    data->keyPath = keyPath;
    data->privKeyPath = privKeyPath;
    data->pubKeyPath = pubKeyPath;
    data->outputPath = outputPath;
    data->passphrase = passphrase;

    data->cipher = cipher;

    data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
