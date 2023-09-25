#ifndef PKEY_ASYNC_KEY_H_INCLUDED
#define PKEY_ASYNC_KEY_H_INCLUDED

#define ERROR_ASYNC_KEY_GEN_ADDRESS ERROR_FUNCTION_ADDRESS(16)

void * async_check(void * arg);

void * async_generate_rsa_key(void * arg);

void * async_generate_on_param_key(void * arg);

#endif // PKEY_ASYNC_KEY_H_INCLUDED
