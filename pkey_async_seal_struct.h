#ifndef PKEY_ASYNC_SEAL_STRUCT_H_INCLUDED
#define PKEY_ASYNC_SEAL_STRUCT_H_INCLUDED

#define ERROR_ASYNC_SEAL_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(26)

struct async_seal_data
{
    const char * inputPath;
    const char * outputPath;
    const char ** pubKeysPath;
    const char * privKeyPath;
    const char * ivPath;
    const char * keyPath;

    const char * passphrase;

    struct asynchronicHandler * handler;

    int keynum;
    int cipher;
    int privCipher;

    unsigned char flags;
};

struct async_seal_data * async_seal_data_new();

int async_seal_data_delete(struct async_seal_data * data);

int async_seal_data_provide(struct async_seal_data * data, const char * inputPath, const char * outputPath, const char ** pubKeysPath,
                            const char * privKeyPath, const char * ivPath, const char * keyPath, const char * passphrase,
                            struct asynchronicHandler * handler, int keynum, int cipher, int privCipher, unsigned char flags);

#endif // PKEY_ASYNC_SEAL_STRUCT_H_INCLUDED
