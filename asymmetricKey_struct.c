#include <string.h>

#include "asymmetricKey_struct.h"

struct asymmetric_key_data * asymmetric_key_data_new()
{
    struct asymmetric_key_data * data = NULL;

    data = (struct asymmetric_key_data*)malloc(sizeof(struct asymmetric_key_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->spinnerDialog = NULL;;
    data->alertDialog = NULL;
    data->passwordDialog = NULL;

    data->keyPubPath = NULL;
    data->keyPrivPath = NULL;
    data->paramPath = NULL;

    data->passphrase = NULL;

    data->keySize = 0;

    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;

    data->flags = 0;

    return data;
}

void asymmetric_key_data_delete(struct asymmetric_key_data * data)
{
    if(data == NULL)
        return;

    asymmetric_key_data_clear(data);
    data->mainWindow = NULL;
    data->spinnerDialog = NULL;;
    data->alertDialog = NULL;
    data->passwordDialog = NULL;

    free(data);
}

void asymmetric_key_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct asymmetric_key_data * data = user_data;

    if(data->keyPrivPath != NULL)
    {
        memset(data->keyPrivPath, 0, sizeof(char)*strlen(data->keyPrivPath));
        free(data->keyPrivPath);
        data->keyPrivPath = NULL;
    }

    if(data->keyPubPath != NULL)
    {
        memset(data->keyPubPath, 0, sizeof(char)*strlen(data->keyPubPath));
        free(data->keyPubPath);
        data->keyPubPath = NULL;
    }

    if(data->paramPath != NULL)
    {
        memset(data->paramPath, 0, sizeof(char)*strlen(data->paramPath));
        free(data->paramPath);
        data->paramPath = NULL;
    }

    if(data->passphrase != NULL)
    {
        memset(data->passphrase, 0, sizeof(char)*strlen(data->passphrase));
        free(data->passphrase);
        data->passphrase = NULL;
    }

    data->keySize = 0;
    data->privAlgorithm = 0;
    data->privBlockMode = 0;
    data->privKeySize = 0;
    data->flags = 0;
}

int asymmetric_key_data_verify(struct asymmetric_key_data * data, unsigned char flags)
{
    if(data == NULL)
        return ASSYMETRIC_KEY_ERROR;

    unsigned char work_mode = (flags & ASSYMETRIC_KEY_MODE_MASK);

    switch(work_mode)
    {
    case ASSYMETRIC_KEY_CHECK:
    {
        if(data->keyPubPath == NULL || data->keyPubPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PUB;
    }

    case ASYMMETRIC_KEY_RSA_GEN:
    {
        if(data->keyPubPath == NULL || data->keyPubPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PUB;
        if(data->keyPrivPath == NULL || data->keyPrivPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PRIV;
        break;
    }

    case ASSYMETRIC_KEY_ON_PARAM_GEN:
    {
        if(data->keyPubPath == NULL || data->keyPubPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PUB;
        if(data->keyPrivPath == NULL || data->keyPrivPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PRIV;
        if(data->paramPath == NULL || data->paramPath[0] == 0)
            return ASSYMETRIC_KEY_NO_KEY_PRIV;
        break;
    }

    }

    return ASSYMETRIC_KEY_OK;
}

void asymmetric_key_data_provide_dialogs(struct asymmetric_key_data * data, GtkWidget * spinnerDialog, GtkWidget * alertDialog,
                                         GtkWidget * passwordDialog)
{
    if(data == NULL)
        return;

    data->spinnerDialog = spinnerDialog;
    data->alertDialog = alertDialog;
    data->passwordDialog = passwordDialog;
}

void asymmetric_key_data_provide_window(struct asymmetric_key_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void asymmetric_key_data_provide_flags(struct asymmetric_key_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}

void asymmetric_key_data_provide_password(struct asymmetric_key_data * data, const char * password)
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
