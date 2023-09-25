#ifndef EVP_ASYNC_DIGEST_STRUCT_H_INCLUDED
#define EVP_ASYNC_DIGEST_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_ASYNC_DIGEST_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(8)

struct async_digest_data
{
    const char * inputPath; // 8
    const char * outputPath; // 16

    ENGINE * engine; // 24

    struct asynchronicHandler * handler; // 32

    int digest_algorithm_position; // 36

    unsigned char flags; // 37
};

struct async_digest_data * async_digest_data_new();

int async_digest_data_delete(struct async_digest_data * digest_data);

int async_digest_data_provide(struct async_digest_data * digest_data, const char * inputPath, const char * outputPath,
                               ENGINE * engine, struct asynchronicHandler * handler, int digest_algorithm_position, unsigned char flags);

#endif // EVP_ASYNC_DIGEST_STRUCT_H_INCLUDED
