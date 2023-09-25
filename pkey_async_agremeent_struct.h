#ifndef PKEY_ASYNC_AGREMEENT_STRUCT_H_INCLUDED
#define PKEY_ASYNC_AGREMEENT_STRUCT_H_INCLUDED

#define ERROR_ASYNC_AGREMEENT_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(30)

struct async_agremeent_data
{
    const char * keyPath;
    const char * privKeyPath;
    const char * pubKeyPath;
    const char * outputPath;
    const char * passphrase;

    int cipher;

    unsigned char flags;
};

struct async_agremeent_data * async_agremeent_data_new();

int async_agremeent_data_delete(struct async_agremeent_data * data);

int async_agremeent_data_provide(struct async_agremeent_data * data, const char * keyPath, const char * privKeyPath, const char * pubKeyPath,
                                 const char * outputPath, const char * passphrase, int cipher, unsigned char flags);

#endif // PKEY_ASYNC_AGREMEENT_STRUCT_H_INCLUDED
