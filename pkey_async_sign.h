#ifndef PKEY_ASYNC_SEAL_H_INCLUDED
#define PKEY_ASYNC_SEAL_H_INCLUDED

#define ERROR_ASYNC_SIGN_ADDRESS ERROR_FUNCTION_ADDRESS(37)

void * async_sign(void * arg);

void * async_verify(void * arg);

#endif // PKEY_ASYNC_SEAL_H_INCLUDED
