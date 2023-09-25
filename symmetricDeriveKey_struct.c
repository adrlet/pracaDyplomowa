#include "symmetricDeriveKey_struct.h"
#include "symmetricEncrypt_struct.h"

struct symmetric_derive_key * symmetric_derive_key_new()
{
    struct symmetric_derive_key * derive_key_data = NULL;

    derive_key_data = (struct symmetric_derive_key*)malloc(sizeof(struct symmetric_derive_key));
    if(derive_key_data == NULL)
        return derive_key_data;

    derive_key_data->mainWindow = NULL;
    derive_key_data->passwordDialog = NULL;
    derive_key_data->spinnerDialog = NULL;
    derive_key_data->alertDialog = NULL;

    derive_key_data->user_data = NULL;
    derive_key_data->user_function = NULL;
    derive_key_data->user_cleaner = NULL;
    derive_key_data->transfer_data = NULL;

    derive_key_data->password = NULL;
    derive_key_data->key = NULL;
    derive_key_data->iv = NULL;
    derive_key_data->saltpath = NULL;

    derive_key_data->keylen = 0;
    derive_key_data->ivlen = 0;
    derive_key_data->saltlen = 0;
    derive_key_data->iterations = 0;

    derive_key_data->flags = 0;

    return derive_key_data;
};

void symmetric_derive_key_delete(struct symmetric_derive_key * derive_key_data)
{
    if(derive_key_data == NULL)
        return;

    symmetric_derive_key_clear(derive_key_data);
    derive_key_data->mainWindow = NULL;
    derive_key_data->passwordDialog = NULL;
    derive_key_data->spinnerDialog = NULL;
    derive_key_data->alertDialog = NULL;

    derive_key_data->user_data = NULL;
    derive_key_data->user_function = NULL;
    derive_key_data->user_cleaner = NULL;
    derive_key_data->transfer_data = NULL;

    free(derive_key_data);
}

void symmetric_derive_key_clear(struct symmetric_derive_key * derive_key_data)
{
    if(derive_key_data == NULL)
        return;

    if(derive_key_data->password != NULL)
    {
        memset(derive_key_data->password, 0,  strlen(derive_key_data->password)*sizeof(char));
        free(derive_key_data->password);
        derive_key_data->password = NULL;
    }

    if(derive_key_data->keylen > 0 && derive_key_data->key != NULL)
    {
        memset(derive_key_data->key, 0, derive_key_data->keylen*sizeof(unsigned char));
        free(derive_key_data->key);
    }
    derive_key_data->key = NULL;
    derive_key_data->keylen = 0;

    if(derive_key_data->ivlen > 0 && derive_key_data->iv != NULL)
    {
        memset(derive_key_data->iv, 0, derive_key_data->ivlen*sizeof(unsigned char));
        free(derive_key_data->iv);
    }
    derive_key_data->iv = NULL;
    derive_key_data->ivlen = 0;

    derive_key_data->saltpath = NULL;
    derive_key_data->saltlen = 0;
    derive_key_data->iterations = 0;

    derive_key_data->flags = 0;
}

void symmetric_derive_key_provide_handlers(struct symmetric_derive_key * derive_key_data, gpointer user_data,
                                           void (*user_function)(void*), void (*user_cleaner)(void*),
                                           void (*transfer_data)(void*, unsigned char*, int, unsigned char*, int))
{
    if(derive_key_data == NULL)
        return;

    derive_key_data->user_data = user_data;
    derive_key_data->user_function = user_function;
    derive_key_data->user_cleaner = user_cleaner;
    derive_key_data->transfer_data = transfer_data;
}

void symmetric_derive_key_provide_proceed_data(struct symmetric_derive_key * derive_key_data,
                                               unsigned char flags, const char * saltpath, int saltlen)
{
    if(derive_key_data == NULL)
        return;

    derive_key_data->flags = flags;

    if(flags & SALT)
    {
        derive_key_data->saltpath = saltpath;
        derive_key_data->saltlen = saltlen;
    }
}

void symmetric_derive_key_provide_password(struct symmetric_derive_key * derive_key_data, const char * password)
{
    if(derive_key_data == NULL || password == NULL)
        return;

    if(derive_key_data->password != NULL)
    {
        memset(derive_key_data->password, 0, strlen(derive_key_data->password)*sizeof(char));
        free(derive_key_data->password);
        derive_key_data->password = NULL;
    }

    if(password[0] != 0)
    {
        derive_key_data->password = (char*)malloc(sizeof(char)*(strlen(password)+1));
        strcpy(derive_key_data->password, password);
    }

    return;
}

void symmetric_derive_key_provide_dialogs(struct symmetric_derive_key * derive_key_data, GtkWidget * passwordDialog, GtkWidget * spinnerDialog, GtkWidget * alertDialog)
{
    if(derive_key_data == NULL)
        return;

    derive_key_data->passwordDialog = passwordDialog;
    derive_key_data->spinnerDialog = spinnerDialog;
    derive_key_data->alertDialog = alertDialog;
}

void symmetric_derive_key_provide_window(struct symmetric_derive_key * derive_key_data, GtkWidget * mainWindow)
{
    if(derive_key_data == NULL)
        return;

    derive_key_data->mainWindow = mainWindow;
}

void symmetric_derive_key_provide_data(struct symmetric_derive_key * derive_key_data, unsigned char * key, int keylen, unsigned char * iv, int ivlen, int iterations)
{
    if(derive_key_data == NULL)
        return;

    if(derive_key_data->key != NULL && derive_key_data->keylen > 0)
    {
        memset(derive_key_data->key, 0, derive_key_data->keylen*sizeof(unsigned char));
        free(derive_key_data->key);
        derive_key_data->key = NULL;
        derive_key_data->keylen = 0;
    }

    if(key != NULL && keylen > 0)
    {
        derive_key_data->keylen = keylen;
        derive_key_data->key = (unsigned char*)malloc(sizeof(unsigned char)*keylen);
        memcpy(derive_key_data->key, key, keylen*sizeof(unsigned char));
    }

    if(derive_key_data->iv != NULL && derive_key_data->ivlen > 0)
    {
        memset(derive_key_data->iv, 0, derive_key_data->ivlen*sizeof(unsigned char));
        free(derive_key_data->iv);
        derive_key_data->iv = NULL;
        derive_key_data->ivlen = 0;
    }

    if(iv != NULL && ivlen > 0)
    {
        derive_key_data->ivlen = ivlen;
        derive_key_data->iv = (unsigned char*)malloc(sizeof(unsigned char)*ivlen);
        memcpy(derive_key_data->iv, iv, ivlen*sizeof(unsigned char));
    }

    if(iterations >= 0)
        derive_key_data->iterations = iterations;

    return;
}

void symmetric_derive_key_provide_lengths(struct symmetric_derive_key * derive_key_data, int keylen, int ivlen)
{
    if(derive_key_data == NULL)
        return;

    derive_key_data->keylen = keylen;
    derive_key_data->ivlen = ivlen;
}
