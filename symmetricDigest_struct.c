#include "symmetricDigest_struct.h"

struct symmetric_digest_data * symmetric_digest_data_new()
{
    struct symmetric_digest_data * data = NULL;

    data = (struct symmetric_digest_data*)malloc(sizeof(struct symmetric_digest_data));
    if(data == NULL)
        return data;

    data->mainWindow = NULL;
    data->progressBox = NULL;
    data->alertDialog = NULL;

    data->cancel = g_cancellable_new();

    data->inputPath = NULL;
    data->outputPath = NULL;

    data->algorithm = 0;

    data->flags = 0;

    return data;
}

void symmetric_digest_data_delete(struct symmetric_digest_data * data)
{
    if(data == NULL)
        return;

    symmetric_digest_data_clear(data);
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

void symmetric_digest_data_clear(void * user_data)
{
    if(user_data == NULL)
        return;

    struct symmetric_digest_data * data = user_data;

    g_cancellable_reset(data->cancel);

    if(data->inputPath != NULL)
    {;
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

    data->algorithm = 0;
    data->flags = 0;
}

int symmetric_digest_data_verify(struct symmetric_digest_data * data)
{
    if(data == NULL)
        return SYMMETRIC_DIGEST_ERROR;

    if(data->inputPath == NULL || data->inputPath[0] == 0)
        return SYMMETRIC_DIGEST_NO_INPUT;
    if(data->outputPath == NULL || data->outputPath[0] == 0)
        return SYMMETRIC_DIGEST_NO_OUTPUT;

    return SYMMETRIC_DIGEST_OK;
}

void symmetric_digest_data_provide_dialogs(struct symmetric_digest_data * data, GtkWidget * progressBox, GtkWidget * alertDialog)
{
    if(data == NULL)
        return;

    data->progressBox = progressBox;
    data->alertDialog = alertDialog;
}

void symmetric_digest_data_provide_window(struct symmetric_digest_data * data, GtkWidget * mainWindow)
{
    if(data == NULL)
        return;

    data->mainWindow = mainWindow;
}

void symmetric_digest_data_provide_flags(struct symmetric_digest_data * data, unsigned char flags)
{
    if(data == NULL)
        return;

    data->flags = data->flags | flags;
}
