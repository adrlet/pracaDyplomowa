#include <string.h>

#include "pkey_async_param_struct.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_ASYNC_PARAM_GEN_STRUCT_ADDRESS, type, code))

struct async_param_data * async_param_data_new()
{
    struct async_param_data * data = NULL;

    data = (struct async_param_data*)malloc(sizeof(struct async_param_data));
    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        goto end;
    }

    data->paramPath = NULL;

    data->keySize = 0;

    data->algorithm = 0;

    data->flags = 0;

end:
    return data;
};

int async_param_data_delete(struct async_param_data * data)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        goto error;
    }

    data->paramPath = NULL;

    data->keySize = 0;

    data->algorithm = 0;

    data->flags = 0;

    free(data);

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}

int async_param_data_provide(struct async_param_data * data, const char * paramPath, int keySize, int algorithm, unsigned char flags)
{
    int iResult = 0;

    if(data == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    data->paramPath = paramPath;

    data->keySize = keySize;

    data->algorithm = algorithm;

    data->flags = flags;

    goto end;

error:
    iResult = -1;
end:
    return iResult;
}
