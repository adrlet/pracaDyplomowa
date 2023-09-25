#ifndef PKEY_PARAM_LOADWRITE_H_INCLUDED
#define PKEY_PARAM_LOADWRITE_H_INCLUDED

#include <openssl/evp.h>
#include <openssl/bio.h>

#define ERROR_PARAM_LOADWRITE_ADDRESS ERROR_FUNCTION_ADDRESS(31)

int extractParameters(EVP_PKEY * params, BIO * output);

EVP_PKEY * loadParameters(EVP_PKEY ** params, BIO * input);

#endif // PKEY_PARAM_LOADWRITE_H_INCLUDED
