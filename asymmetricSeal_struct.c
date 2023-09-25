#include <string.h>

#include "asymmetricSeal_struct.h"

struct asymmetric_seal_data * asymmetric_seal_data_new()
{
    struct asymmetric_seal_data * data = NULL;

    data = (struct asymmetric_seal_data*)malloc(sizeof(struct asymmetric_seal_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->progressBox = NULL;
    data->alertDialog = NULL;
    data->passwordDialog = NULL;

    data->passphrase = NULL;
    data->inputPath = NULL;
    data->outputPath = NULL;
    data->pubKeyPath = NULL;
    data->privKeyPath = NULL;
    data->ivPath = NULL;
    data->keyPath = NULL;

    data->cancel = g_cancellable_new();

    data->keynum = 0;

    data->algorithm = 0;
    data->blockMode = 0;
    data->keySize = 0;

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0;

    return data;
}

void asymmetric_seal_data_delete(struct asymmetric_seal_data * data)
{
    if(data == NULL)
        return;

    asymmetric_seal_data_clear(data);
    data->mainWindow = NULL; // 8
    data->progressBox = NULL; // 16
    data->alertDialog = NULL; // 24
    data->passwordDialog = NULL; // 3

    if(data->cancel != NULL)
    {
        g_object_unref(data->cancel);
        data->cancel = NULL;
    }

    free(data);
}

void asymmetric_seal_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct asymmetric_seal_data * data = user_data;

    if(data->passphrase != NULL)
    {
        memset(data->passphrase, 0, sizeof(char)*strlen(data->passphrase));
        free(data->passphrase);
        data->passphrase = NULL;
    }

    if(data->inputPath != NULL)
    {
        memset(data->inputPath, 0, sizeof(char)*strlen(data->inputPath));
        free(data->inputPath);
        data->inputPath = NULL;
    }

    if(data->outputPath != NULL)
    {
        memset(data->outputPath, 0, sizeof(char)*strlen(data->outputPath));
        free(data->outputPath);
        data->outputPath = NULL;
    }

    if(data->pubKeyPath != NULL)
    {
        memset(data->pubKeyPath, 0, sizeof(char)*strlen(data->pubKeyPath));
        free(data->pubKeyPath);
        data->pubKeyPath = NULL;
    }

    if(data->privKeyPath != NULL)
    {
        memset(data->privKeyPath, 0, sizeof(char)*strlen(data->privKeyPath));
        free(data->privKeyPath);
        data->privKeyPath = NULL;
    }

    if(data->ivPath != NULL)
    {
        memset(data->ivPath, 0, sizeof(char)*strlen(data->ivPath));
        free(data->ivPath);
        data->ivPath = NULL;
    }

    if(data->keyPath != NULL)
    {
        memset(data->keyPath, 0, sizeof(char)*strlen(data->keyPath));
        free(data->keyPath);
        data->keyPath = NULL;
    }

    g_cancellable_reset(data->cancel);

    data->algorithm = 0;
    data->blockMode = 0;
    data->keySize = 0;

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0;
}

int asymmetric_seal_data_verify(struct asymmetric_seal_data * data)
{
    int mode = 0;

    if(data == NULL)
        return ASSYMETRIC_SEAL_ERROR;

    if(data->inputPath == NULL || data->inputPath[0] == 0)
        return ASSYMETRIC_SEAL_NO_INPUT;
    if(data->outputPath == NULL || data->outputPath[0] == 0)
        return ASSYMETRIC_SEAL_NO_OUTPUT;
    if(data->ivPath == NULL && data->ivPath[0] == 0)
        return ASSYMETRIC_SEAL_NO_IV;
    if(data->keyPath == NULL && data->keyPath[0] == 0)
        return ASSYMETRIC_SEAL_NO_SESSION;

    mode = (data->flags & ASSYMETRIC_SEAL_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_SEAL_SEAL:
        if(data->pubKeyPath == NULL && data->pubKeyPath[0] == 0)
            return ASSYMETRIC_SEAL_NO_PUBKEY;
        break;
    case ASSYMETRIC_SEAL_OPEN:
        if(data->privKeyPath == NULL && data->privKeyPath[0] == 0)
            return ASSYMETRIC_SEAL_NO_PRIVKEY;
        break;
    }

    return ASSYMETRIC_SEAL_OK;
}

void asymmetric_seal_data_provide_dialogs(struct asymmetric_seal_data * data, GtkWidget * progressBox, GtkWidget * alertDialog,
                                          GtkWidget * passwordDialog)
{
    if(data == NULL)
        return;

    data->progressBox = progressBox;
    data->alertDialog = alertDialog;
    data->passwordDialog = passwordDialog;
}

void asymmetric_seal_data_provide_window(struct asymmetric_seal_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void asymmetric_seal_data_provide_flags(struct asymmetric_seal_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}

void asymmetric_seal_data_provide_password(struct asymmetric_seal_data * data, const char * password)
{
    if(data == NULL)
        return;

    if(data->passphrase != NULL)
    {
        memset(data->passphrase, 0, sizeof(char)*strlen(data->passphrase));
        free(data->passphrase);
        data->passphrase = NULL;
    }

    if(password != NULL && password[0] != 0)
    {
        data->passphrase = (char*)malloc(sizeof(char)*(strlen(password)+1));
        strcpy(data->passphrase, password);
    }
}
