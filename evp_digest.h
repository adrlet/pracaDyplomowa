#ifndef EVP_DIGEST_H_INCLUDED
#define EVP_DIGEST_H_INCLUDED

#include <openssl/evp.h>

#include "thread_asynchronichandler.h"

#define ERROR_DIGEST_ADDRESS ERROR_FUNCTION_ADDRESS(12)

int digest(BIO * input, BIO * output, const EVP_MD * algorithm, struct asynchronicHandler * handler, ENGINE * engine);

#endif // EVP_DIGEST_H_INCLUDED
