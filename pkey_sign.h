#ifndef PKEY_SIGN_H_INCLUDED
#define PKEY_SIGN_H_INCLUDED

#include <openssl/evp.h>
#include <openssl/bio.h>

#include "thread_asynchronichandler.h"
#include "pkey_sign_struct.h"

#define ERROR_SIGN_ADDRESS ERROR_FUNCTION_ADDRESS(35)

int sign(BIO * input, BIO * output, struct signData * signParameters, struct asynchronicHandler * handler, ENGINE * engine);

int asymmetricVerify(BIO * input, BIO * signInput, struct signData * signParameters, struct asynchronicHandler * handler, ENGINE * engine, int * result);

#endif // PKEY_SIGN_H_INCLUDED
