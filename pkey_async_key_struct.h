#ifndef PKEY_ASYNC_KEY_STRUCT_H_INCLUDED
#define PKEY_ASYNC_KEY_STRUCT_H_INCLUDED

#define ERROR_ASYNC_KEY_GEN_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(17)

struct async_key_data
{
    const char * passphrase; // 8
    const char * pubKeyPath; // 16
    const char * privKeyPath; // 24
    const char * paramPath; // 32

    int keySize; // 36
    int cipherAlgorithm; // 40

    unsigned char flags; // 41
};

struct async_key_data * async_key_data_new();

int async_key_data_delete(struct async_key_data * data);

int async_key_data_provide(struct async_key_data * data, const char * passphrase, const char * pubKeyPath, const char * privKeyPath,
                           const char * paramPath, int keySize, int cipherAlgorithm, unsigned char flags);

#endif // PKEY_ASYNC_KEY_STRUCT_H_INCLUDED
