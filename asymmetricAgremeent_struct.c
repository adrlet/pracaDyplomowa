#include <string.h>

#include "asymmetricAgremeent_struct.h"

struct asymmetric_agremeent_data * asymmetric_agremeent_data_new()
{
    struct asymmetric_agremeent_data * data = NULL;

    data = (struct asymmetric_agremeent_data*)malloc(sizeof(struct asymmetric_agremeent_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->alertDialog = NULL;
    data->spinnerDialog = NULL;
    data->passwordDialog = NULL;

    data->passphrase = NULL;
    data->keyPath = NULL;
    data->privKeyPath = NULL;
    data->pubKeyPath = NULL;
    data->outputPath = NULL;

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0;

    return data;
}

void asymmetric_agremeent_data_delete(struct asymmetric_agremeent_data * data)
{
    if(data == NULL)
        return;

    asymmetric_agremeent_data_clear(data);
    data->mainWindow = NULL; // 8
    data->spinnerDialog = NULL; // 16
    data->alertDialog = NULL; // 24
    data->passwordDialog = NULL;

    free(data);
}

void asymmetric_agremeent_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct asymmetric_agremeent_data * data = user_data;

    if(data->passphrase != NULL)
    {
        memset(data->passphrase, 0, sizeof(char)*strlen(data->passphrase));
        free(data->passphrase);
        data->passphrase = NULL;
    }

    if(data->keyPath != NULL)
    {
        memset(data->keyPath, 0, sizeof(char)*strlen(data->keyPath));
        free(data->keyPath);
        data->keyPath = NULL;
    }

    if(data->privKeyPath != NULL)
    {
        memset(data->privKeyPath, 0, sizeof(char)*strlen(data->privKeyPath));
        free(data->privKeyPath);
        data->privKeyPath = NULL;
    }

    if(data->pubKeyPath != NULL)
    {
        memset(data->pubKeyPath, 0, sizeof(char)*strlen(data->pubKeyPath));
        free(data->pubKeyPath);
        data->pubKeyPath = NULL;
    }

    if(data->outputPath != NULL)
    {
        memset(data->outputPath, 0, sizeof(char)*strlen(data->outputPath));
        free(data->outputPath);
        data->outputPath = NULL;
    }

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0; // 73
}

int asymmetric_agremeent_data_verify(struct asymmetric_agremeent_data * data)
{
    if(data == NULL)
        return ASSYMETRIC_AGREMEENT_ERROR;

    if(data->keyPath == NULL || data->keyPath[0] == 0)
        return ASSYMETRIC_AGREMEENT_NO_KEY;
    if(data->privKeyPath == NULL || data->privKeyPath[0] == 0)
        return ASSYMETRIC_AGREMEENT_NO_PUBKEY;
    if(data->pubKeyPath == NULL || data->pubKeyPath[0] == 0)
        return ASSYMETRIC_AGREMEENT_NO_PUBKEY;
    if(data->outputPath == NULL || data->outputPath[0] == 0)
        return ASSYMETRIC_AGREMEENT_NO_OUTPUT;

    return ASSYMETRIC_AGREMEENT_OK;
}

void asymmetric_agremeent_data_provide_dialogs(struct asymmetric_agremeent_data * data, GtkWidget * spinnerDialog,
                                               GtkWidget * alertDialog, GtkWidget * passwordDialog)
{
    if(data == NULL)
        return;

    data->spinnerDialog = spinnerDialog;
    data->alertDialog = alertDialog;
    data->passwordDialog = passwordDialog;
}

void asymmetric_agremeent_data_provide_window(struct asymmetric_agremeent_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void asymmetric_agremeent_data_provide_flags(struct asymmetric_agremeent_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}

void asymmetric_agremeent_data_provide_password(struct asymmetric_agremeent_data * data, const char * password)
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
