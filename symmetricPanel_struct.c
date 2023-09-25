#include "symmetricPanel_struct.h"
#include "symmetricEncrypt_struct.h"
#include "symmetric_cipher_map.h"

struct symmetric_wrapper * symmetric_wrapper_new()
{
    struct symmetric_wrapper * wrapper = NULL;

    wrapper = (struct symmetric_wrapper*)malloc(sizeof(struct symmetric_wrapper));
    if(wrapper == NULL)
        return wrapper;

    wrapper->mainWindow = NULL;

    wrapper->inputEntry = NULL;
    wrapper->outputEntry = NULL;

    wrapper->algorithmDropDown = NULL;
    wrapper->blockModeDropDown = NULL;
    wrapper->keySizeDropDown = NULL;

    wrapper->base64Check = NULL;
    wrapper->saltCheck = NULL;

    wrapper->derive_key_data = NULL;
    wrapper->data = NULL;

    return wrapper;
};

void symmetric_wrapper_delete(struct symmetric_wrapper * wrapper)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = NULL;

    wrapper->inputEntry = NULL;
    wrapper->outputEntry = NULL;

    wrapper->algorithmDropDown = NULL;
    wrapper->blockModeDropDown = NULL;
    wrapper->keySizeDropDown = NULL;

    wrapper->base64Check = NULL;
    wrapper->saltCheck = NULL;

    wrapper->derive_key_data = NULL;
    wrapper->data = NULL;

    free(wrapper);
}

void symmetric_wrapper_provide_window(struct symmetric_wrapper * wrapper, GtkWidget * mainWindow)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = mainWindow;
}

void symmetric_wrapper_provide_widgets(struct symmetric_wrapper * wrapper, GtkWidget * inputEntry, GtkWidget * outputEntry,
                                       GtkWidget * algorithmDropDown, GtkWidget * blockModeDropDown, GtkWidget * keySizeDropDown,
                                       GtkWidget * base64Check, GtkWidget * saltCheck)
{
    if(wrapper == NULL)
        return;

    wrapper->inputEntry = inputEntry;
    wrapper->outputEntry = outputEntry;

    wrapper->algorithmDropDown = algorithmDropDown;
    wrapper->blockModeDropDown = blockModeDropDown;
    wrapper->keySizeDropDown = keySizeDropDown;

    wrapper->base64Check = base64Check;
    wrapper->saltCheck = saltCheck;
}

void symmetric_wrapper_provide_data(struct symmetric_wrapper * wrapper, struct symmetric_data * data)
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
        data->algorithm = CIPHER_ALGORITHM_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->algorithmDropDown))+1));

    if(wrapper->blockModeDropDown != NULL)
        data->blockmode = CIPHER_BLOCK_MODE_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->blockModeDropDown))+1));

    if(wrapper->keySizeDropDown != NULL)
    {
        if(data->algorithm == CIPHER_ALGORITHM_AES || data->algorithm == CIPHER_ALGORITHM_CAMELLIA)
            data->keysize = CIPHER_KEY_SIZE_ADDRESS((gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->keySizeDropDown))+1));
        else
            data->keysize = CIPHER_KEY_SIZE_ADDRESS(gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->keySizeDropDown)));
    }

    if(wrapper->base64Check != NULL)
        if(gtk_check_button_get_active(GTK_CHECK_BUTTON(wrapper->base64Check)) == TRUE)
            data->flags = data->flags | BASE64;

    if(wrapper->saltCheck != NULL)
        if(gtk_check_button_get_active(GTK_CHECK_BUTTON(wrapper->saltCheck)) == TRUE)
            data->flags = data->flags | SALT;

    data->cancel = g_cancellable_new();
}
