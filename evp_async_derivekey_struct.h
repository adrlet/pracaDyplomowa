#ifndef EVP_ASYNC_DERIVEKEY_STRUCT_H_INCLUDED
#define EVP_ASYNC_DERIVEKEY_STRUCT_H_INCLUDED

#define ERROR_ASYNC_DERIVE_KEY_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(4)

struct async_deriveKey_data
{
    const char * passphrase; // 8
    unsigned char * key; // 16
    unsigned char * iv; // 24
    const char * saltpath; // 32

    int digest_algorithm_position; // 36
    int keylen; // 40
    int ivlen; // 44
    int saltlen; // 48

    unsigned char flags; // 49
};

struct async_deriveKey_data * async_deriveKey_data_new();

int async_deriveKey_data_delete(struct async_deriveKey_data * deriveKey_data);

int async_deriveKey_data_provide(struct async_deriveKey_data * deriveKey_data, char * passphrase, const char * saltpath,
                                 int digest_algorithm_position, int keylen, int ivlen, int saltlen, unsigned char flags);

int async_deriveKey_data_init_buffers(struct async_deriveKey_data * deriveKey_data);

#endif // EVP_ASYNC_DERIVEKEY_STRUCT_H_INCLUDED
