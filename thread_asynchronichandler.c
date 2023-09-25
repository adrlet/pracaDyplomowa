#include <openssl/evp.h>

#include "thread_asynchronichandler.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_THREAD_ASYNCHRONIC_HANDLER_ADDRESS, type, code))

struct asynchronicHandler * setupAsynchronicHandler()
{
    struct asynchronicHandler * handler = NULL;

    handler = (struct asynchronicHandler*)malloc(sizeof(struct asynchronicHandler));
    if(handler == NULL)
    {
        //writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    handler->interrupt = 0;
    handler->processed = 0;

    return handler;
}

void freeAsynchronicHandler(struct asynchronicHandler * handler)
{
    if(handler == NULL)
        return;

    handler->interrupt = 0;
    handler->processed = 0;

    free(handler);
}
