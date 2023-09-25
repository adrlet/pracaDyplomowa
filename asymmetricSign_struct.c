#include <string.h>

#include "asymmetricSign_struct.h"

struct asymmetric_sign_data * asymmetric_sign_data_new()
{
    struct asymmetric_sign_data * data = NULL;

    data = (struct asymmetric_sign_data*)malloc(sizeof(struct asymmetric_sign_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->progressBox = NULL;
    data->alertDialog = NULL;
    data->passwordDialog = NULL;

    data->passphrase = NULL;
    data->inputPath = NULL;
    data->outputPath = NULL;
    data->keyPath = NULL;

    data->cancel = g_cancellable_new();

    data->digest = 0;

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0;

    return data;
}

void asymmetric_sign_data_delete(struct asymmetric_sign_data * data)
{
    if(data == NULL)
        return;

    asymmetric_sign_data_clear(data);
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

void asymmetric_sign_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct asymmetric_sign_data * data = user_data;

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

    if(data->keyPath != NULL)
    {
        memset(data->keyPath, 0, sizeof(char)*strlen(data->keyPath));
        free(data->keyPath);
        data->keyPath = NULL;
    }

    g_cancellable_reset(data->cancel);

    data->digest = 0; // 68

    data->privAlgorithm = 0; // 68
    data->privBlockMode = 0; // 72
    data->privKeySize = 0; // 72

    data->flags = 0; // 73
}

int asymmetric_sign_data_verify(struct asymmetric_sign_data * data)
{
    if(data == NULL)
        return ASSYMETRIC_SIGN_ERROR;

    if(data->inputPath == NULL || data->inputPath[0] == 0)
        return ASSYMETRIC_SIGN_NO_INPUT;
    if(data->outputPath == NULL || data->outputPath[0] == 0)
        return ASSYMETRIC_SIGN_NO_OUTPUT;
    if(data->keyPath == NULL && data->keyPath[0] == 0)
        return ASSYMETRIC_SIGN_NO_KEY_PATH;

    return ASSYMETRIC_SIGN_OK;
}

void asymmetric_sign_data_provide_dialogs(struct asymmetric_sign_data * data, GtkWidget * progressBox, GtkWidget * alertDialog,
                                          GtkWidget * passwordDialog)
{
    if(data == NULL)
        return;

    data->progressBox = progressBox;
    data->alertDialog = alertDialog;
    data->passwordDialog = passwordDialog;
}

void asymmetric_sign_data_provide_window(struct asymmetric_sign_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void asymmetric_sign_data_provide_flags(struct asymmetric_sign_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}

void asymmetric_sign_data_provide_password(struct asymmetric_sign_data * data, const char * password)
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
