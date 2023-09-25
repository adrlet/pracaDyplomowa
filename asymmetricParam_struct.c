#include <string.h>

#include "asymmetricParam_struct.h"

struct asymmetric_param_data * asymmetric_param_data_new()
{
    struct asymmetric_param_data * data = NULL;

    data = (struct asymmetric_param_data*)malloc(sizeof(struct asymmetric_param_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->spinnerDialog = NULL;;
    data->alertDialog = NULL;

    data->paramPath = NULL;

    data->keySize = 0;

    data->algorithm = 0;

    data->flags = 0;

    return data;
}

void asymmetric_param_data_delete(struct asymmetric_param_data * data)
{
    if(data == NULL)
        return;

    asymmetric_param_data_clear(data);
    data->mainWindow = NULL;
    data->spinnerDialog = NULL;;
    data->alertDialog = NULL;

    free(data);
}

void asymmetric_param_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct asymmetric_param_data * data = user_data;

    if(data->paramPath != NULL)
    {
        memset(data->paramPath, 0, sizeof(char)*strlen(data->paramPath));
        free(data->paramPath);
        data->paramPath = NULL;
    }

    data->keySize = 0;
    data->algorithm = 0;
    data->flags = 0;
}

int asymmetric_param_data_verify(struct asymmetric_param_data * data)
{
    if(data == NULL)
        return ASSYMETRIC_PARAM_ERROR;

    if(data->paramPath != NULL && data->paramPath[0] == 0)
        return ASSYMETRIC_PARAM_NO_PARAM;

    return ASSYMETRIC_PARAM_OK;
}

void asymmetric_param_data_provide_dialogs(struct asymmetric_param_data * data, GtkWidget * spinnerDialog, GtkWidget * alertDialog)
{
    if(data == NULL)
        return;

    data->spinnerDialog = spinnerDialog;
    data->alertDialog = alertDialog;
}

void asymmetric_param_data_provide_window(struct asymmetric_param_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void asymmetric_param_data_provide_flags(struct asymmetric_param_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}
