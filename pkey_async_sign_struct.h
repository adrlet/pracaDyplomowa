#ifndef PKEY_ASYNC_SIGN_STRUCT_H_INCLUDED
#define PKEY_ASYNC_SIGN_STRUCT_H_INCLUDED

#define ERROR_ASYNC_SIGN_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(38)

struct async_sign_data
{
    const char * inputPath;
    const char * outputPath;
    const char * keyPath;

    const char * passphrase;

    struct asynchronicHandler * handler;

    int digest;
    int cipher;

    unsigned char flags;
};

struct async_sign_data * async_sign_data_new();

int async_sign_data_delete(struct async_sign_data * data);

int async_sign_data_provide(struct async_sign_data * data, const char * inputPath, const char * outputPath, const char * keyPath,
                            const char * passphrase, struct asynchronicHandler * handler,
                            int digest, int cipher, unsigned char flags);

#endif // PKEY_ASYNC_SIGN_STRUCT_H_INCLUDED
