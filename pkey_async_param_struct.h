#ifndef PKEY_ASYNC_PARAM_STRUCT_H_INCLUDED
#define PKEY_ASYNC_PARAM_STRUCT_H_INCLUDED

#define ERROR_ASYNC_PARAM_GEN_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(20)

struct async_param_data
{
    const char * paramPath; // 8

    int keySize; // 12

    int algorithm; // 16

    unsigned char flags; // 17
};

struct async_param_data * async_param_data_new();

int async_param_data_delete(struct async_param_data * data);

int async_param_data_provide(struct async_param_data * data, const char * paramPath, int keySize, int algorithm, unsigned char flags);

#endif // PKEY_ASYNC_PARAM_STRUCT_H_INCLUDED
