#ifndef PKEY_PARAMGEN_H_INCLUDED
#define PKEY_PARAMGEN_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_PARAM_GEN_ADDRESS ERROR_FUNCTION_ADDRESS(18)

int EVP_PKEY_CTX_set_ec_paramgen_curve_nid_func(EVP_PKEY_CTX * ctx, int bits);

int EVP_PKEY_CTX_set_dsa_paramgen_bits_func(EVP_PKEY_CTX * ctx, int bits);

int EVP_PKEY_CTX_set_dh_paramgen_prime_len_func(EVP_PKEY_CTX * ctx, int bits);

int generateParameters(int type, int count, int bit, EVP_PKEY ** parameters, ENGINE * engine);

#endif // PKEY_PARAMGEN_H_INCLUDED
