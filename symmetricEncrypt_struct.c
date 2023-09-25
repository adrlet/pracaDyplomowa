#include "symmetricEncrypt_struct.h"

struct symmetric_data * symmetric_data_new()
{
    struct symmetric_data * data = NULL;

    data = (struct symmetric_data*)malloc(sizeof(struct symmetric_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->progressBox = NULL;
    data->alertDialog = NULL;

    data->cancel = g_cancellable_new();

    data->inputPath = NULL;
    data->outputPath = NULL;

    data->key = NULL;
    data->iv = NULL;

    data->algorithm = 0;
    data->blockmode = 0;
    data->keysize = 0;

    data->keylen = 0;
    data->ivlen = 0;

    data->flags = 0;

    return data;
}

void symmetric_data_delete(struct symmetric_data * data)
{
    if(data == NULL)
        return;

    symmetric_data_clear(data);
    data->mainWindow = NULL;
    data->progressBox = NULL;
    data->alertDialog = NULL;

    if(data->cancel != NULL)
    {
        g_object_unref(data->cancel);
        data->cancel = NULL;
    }

    free(data);
}

void symmetric_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct symmetric_data * data = user_data;

    g_cancellable_reset(data->cancel);

    if(data->inputPath != NULL)
    {
        printf("Rakonik inputPath\n");
        memset(data->inputPath, 0, sizeof(char)*strlen(data->inputPath));
        printf("Rakonik inputPath\n");
        free(data->inputPath);
        printf("Rakonik inputPath\n");
        data->inputPath = NULL;
    }

    if(data->outputPath != NULL)
    {
        printf("Rakonik outputPath\n");
        memset(data->outputPath, 0, sizeof(char)*strlen(data->outputPath));
        free(data->outputPath);
        data->outputPath = NULL;
    }

    if(data->key != NULL && data->keylen > 0)
    {
        printf("Rakonik key\n");
        memset(data->key, 0, sizeof(unsigned char)*data->keylen);
        free(data->key);
    }
    data->key = NULL;
    data->keylen = 0;

    if(data->iv != NULL && data->ivlen > 0)
    {
        printf("Rakonik iv\n");
        memset(data->iv, 0, sizeof(unsigned char)*data->ivlen);
        free(data->iv);
    }
    data->iv = NULL;
    data->ivlen = 0;

    data->algorithm = 0;
    data->blockmode = 0;
    data->keysize = 0;

    data->flags = 0;
}

int symmetric_data_verify(struct symmetric_data * data)
{
    if(data == NULL)
        return SYMMETRIC_PANEL_ERROR;

    if(data->inputPath == NULL || data->inputPath[0] == 0)
        return SYMMETRIC_PANEL_NO_INPUT;
    if(data->outputPath == NULL || data->outputPath[0] == 0)
        return SYMMETRIC_PANEL_NO_OUTPUT;

    return SYMMETRIC_PANEL_OK;
}

void symmetric_data_provide_data(void * user_data, unsigned char * key, int keylen, unsigned char * iv, int ivlen)
{
    if(user_data == NULL)
        return;

    struct symmetric_data * data = user_data;

    if(data->key != NULL && data->keylen > 0)
    {
        memset(data->key, 0, data->keylen*sizeof(unsigned char));
        free(data->key);
        data->key = NULL;
        data->keylen = 0;
    }

    if(key != NULL && keylen > 0)
    {
        data->key = (unsigned char*)malloc(sizeof(unsigned char)*keylen);
        data->keylen = keylen;
        memcpy(data->key, key, keylen*sizeof(unsigned char));
    }

    if(data->iv != NULL && data->ivlen > 0)
    {
        memset(data->iv, 0, data->ivlen*sizeof(unsigned char));
        free(data->iv);
        data->iv = NULL;
        data->ivlen = 0;
    }

    if(iv != NULL && ivlen > 0)
    {
        data->iv = (unsigned char*)malloc(sizeof(unsigned char)*ivlen);
        data->ivlen = ivlen;
        memcpy(data->iv, iv, ivlen*sizeof(unsigned char));
    }
}

void symmetric_data_provide_dialogs(struct symmetric_data * data, GtkWidget * progressBox, GtkWidget * alertDialog)
{
    if(data == NULL)
        return;

    data->progressBox = progressBox;
    data->alertDialog = alertDialog;
}

void symmetric_data_provide_window(struct symmetric_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void symmetric_data_provide_flags(struct symmetric_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}
