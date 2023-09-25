#include "symmetricDigestPanel_struct.h"
#include "symmetricDigest_struct.h"

struct symmetric_digest_wrapper * symmetric_digest_wrapper_new()
{
    struct symmetric_digest_wrapper * wrapper = NULL;

    wrapper = (struct symmetric_digest_wrapper*)malloc(sizeof(struct symmetric_digest_wrapper));
    if(wrapper == NULL)
        return wrapper;

    wrapper->mainWindow = NULL;

    wrapper->inputEntry = NULL;
    wrapper->outputEntry = NULL;

    wrapper->algorithmDropDown = NULL;

    wrapper->base64Check = NULL;

    wrapper->data = NULL;

    return wrapper;
};

void symmetric_digest_wrapper_delete(struct symmetric_digest_wrapper * wrapper)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = NULL;

    wrapper->inputEntry = NULL;
    wrapper->outputEntry = NULL;

    wrapper->algorithmDropDown = NULL;

    wrapper->base64Check = NULL;

    wrapper->data = NULL;

    free(wrapper);
}

void symmetric_digest_wrapper_provide_window(struct symmetric_digest_wrapper * wrapper, GtkWidget * mainWindow)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = mainWindow;
}

void symmetric_digest_wrapper_provide_widgets(struct symmetric_digest_wrapper * wrapper, GtkWidget * inputEntry, GtkWidget * outputEntry,
                                       GtkWidget * algorithmDropDown, GtkWidget * base64Check)
{
    if(wrapper == NULL)
        return;

    wrapper->inputEntry = inputEntry;
    wrapper->outputEntry = outputEntry;

    wrapper->algorithmDropDown = algorithmDropDown;

    wrapper->base64Check = base64Check;
}

void symmetric_digest_wrapper_provide_data(struct symmetric_digest_wrapper * wrapper, struct symmetric_digest_data * data)
{
    GtkEntryBuffer * inputEntryBuffer = NULL;
    GtkEntryBuffer * outputEntryBuffer = NULL;

    const char * inputPath = NULL;
    const char * outputPath = NULL;

    if(wrapper == NULL || data == NULL)
        return;

    if(wrapper->inputEntry != NULL)
    {
        inputEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->inputEntry));
        inputPath = gtk_entry_buffer_get_text(inputEntryBuffer);
        if(inputPath != NULL && inputPath[0] != 0)
        {
            data->inputPath = (char*)malloc(sizeof(char)*(strlen(inputPath)+1));
            strcpy(data->inputPath, inputPath);
            printf("%s\n", data->inputPath);
        }
    }

    if(wrapper->outputEntry != NULL)
    {
        outputEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->outputEntry));
        outputPath = gtk_entry_buffer_get_text(outputEntryBuffer);
        if(outputPath != NULL && outputPath[0] != 0)
        {
            data->outputPath = (char*)malloc(sizeof(char)*(strlen(outputPath)+1));
            strcpy(data->outputPath, outputPath);
        }
    }

    if(wrapper->algorithmDropDown != NULL)
        data->algorithm = gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->algorithmDropDown));

    if(wrapper->base64Check != NULL)
        if(gtk_check_button_get_active(GTK_CHECK_BUTTON(wrapper->base64Check)) == TRUE)
            data->flags = data->flags | BASE64;

    data->cancel = g_cancellable_new();
}
