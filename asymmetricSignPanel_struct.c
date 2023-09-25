#include <string.h>

#include "asymmetricSignPanel_struct.h"
#include "widgetManagment.h"
#include "symmetric_cipher_map.h"

struct asymmetric_sign_wrapper * asymmetric_sign_wrapper_new()
{
    struct asymmetric_sign_wrapper * wrapper = NULL;

    wrapper = (struct asymmetric_sign_wrapper*)malloc(sizeof(struct asymmetric_sign_wrapper));
    if(wrapper == NULL)
        return wrapper;

    wrapper->mainWindow = NULL;

    wrapper->inputPathEntry = NULL;
    wrapper->outputPathEntry = NULL;
    wrapper->keyPathEntry = NULL;

    wrapper->digestDropDown = NULL;
    wrapper->base64Check = NULL;

    wrapper->privAlgorithmDropDown = NULL;
    wrapper->privBlockModeDropDown = NULL;
    wrapper->privKeySizeDropDown = NULL;

    wrapper->data = NULL;

    return wrapper;
};

void asymmetric_sign_wrapper_delete(struct asymmetric_sign_wrapper * wrapper)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = NULL;

    wrapper->inputPathEntry = NULL;
    wrapper->outputPathEntry = NULL;
    wrapper->keyPathEntry = NULL;

    wrapper->digestDropDown = NULL;
    wrapper->base64Check = NULL;

    wrapper->privAlgorithmDropDown = NULL;
    wrapper->privBlockModeDropDown = NULL;
    wrapper->privKeySizeDropDown = NULL;

    wrapper->data = NULL;

    free(wrapper);
}

void asymmetric_sign_wrapper_provide_window(struct asymmetric_sign_wrapper * wrapper, GtkWidget * mainWindow)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = mainWindow;
}

void asymmetric_sign_wrapper_provide_widgets(struct asymmetric_sign_wrapper * wrapper, GtkWidget * inputPathEntry,
                                             GtkWidget * outputPathEntry, GtkWidget * keyPathEntry,
                                             GtkWidget * digestDropDown, GtkWidget * base64Check,
                                             GtkWidget * privAlgorithmDropDown, GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown)
{
    if(wrapper == NULL)
        return;

    wrapper->inputPathEntry = inputPathEntry;
    wrapper->outputPathEntry = outputPathEntry;
    wrapper->keyPathEntry = keyPathEntry;

    wrapper->digestDropDown = digestDropDown;
    wrapper->base64Check = base64Check;

    wrapper->privAlgorithmDropDown = privAlgorithmDropDown;
    wrapper->privBlockModeDropDown = privBlockModeDropDown;
    wrapper->privKeySizeDropDown = privKeySizeDropDown;
}

void asymmetric_sign_wrapper_provide_data(struct asymmetric_sign_wrapper * wrapper, struct asymmetric_sign_data * data)
{
    GtkEntryBuffer * entryBuffer = NULL;
    const char * path = NULL;
    int i = 0;

    if(wrapper == NULL || data == NULL)
        return;

    if(wrapper->inputPathEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->inputPathEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->inputPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->inputPath, path);
            printf("%s\n", data->inputPath);
        }
    }

    if(wrapper->outputPathEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->outputPathEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->outputPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->outputPath, path);
            printf("%s\n", data->outputPath);
        }
    }

    if(wrapper->keyPathEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->keyPathEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->keyPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->keyPath, path);
            printf("%s\n", data->keyPath);
        }
    }

    if(wrapper->digestDropDown != NULL)
        data->digest = gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->digestDropDown));

    if(wrapper->base64Check != NULL)
        if(gtk_check_button_get_active(GTK_CHECK_BUTTON(wrapper->base64Check)) == TRUE)
            data->flags = data->flags | SIGN_BASE64;

    if(wrapper->privAlgorithmDropDown != NULL)
        data->privAlgorithm = CIPHER_ALGORITHM_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->privAlgorithmDropDown))+1));

    if(wrapper->privBlockModeDropDown != NULL)
        data->privBlockMode = CIPHER_BLOCK_MODE_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->privBlockModeDropDown))+1));

    if(wrapper->privKeySizeDropDown != NULL)
    {
        if(data->privAlgorithm == CIPHER_ALGORITHM_AES || data->privAlgorithm == CIPHER_ALGORITHM_CAMELLIA)
            data->privKeySize = CIPHER_KEY_SIZE_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->privKeySizeDropDown))+1));
        else
            data->privKeySize = CIPHER_KEY_SIZE_ADDRESS(gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->privKeySizeDropDown)));
    }
}
