#ifndef THREAD_ASYNCHRONICHANDLER_H_INCLUDED
#define THREAD_ASYNCHRONICHANDLER_H_INCLUDED

#define ERROR_THREAD_ASYNCHRONIC_HANDLER_ADDRESS ERROR_FUNCTION_ADDRESS(9)

struct asynchronicHandler
{
    unsigned char interrupt;
    unsigned long long processed;
};

struct asynchronicHandler * setupAsynchronicHandler();

void freeAsynchronicHandler(struct asynchronicHandler * handler);

#endif // THREAD_ASYNCHRONICHANDLER_H_INCLUDED
