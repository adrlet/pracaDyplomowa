#include <string.h>

#include "asymmetricKeyPanel_struct.h"
#include "widgetManagment.h"
#include "symmetric_cipher_map.h"

struct asymmetric_key_wrapper * asymmetric_key_wrapper_new()
{
    struct asymmetric_key_wrapper * wrapper = NULL;

    wrapper = (struct asymmetric_key_wrapper*)malloc(sizeof(struct asymmetric_key_wrapper));
    if(wrapper == NULL)
        return wrapper;

    wrapper->mainWindow = NULL;

    wrapper->keyPubInputEntry = NULL;
    wrapper->keyPrivInputEntry = NULL;
    wrapper->paramInputEntry = NULL;

    wrapper->keySizeEntry = NULL;

    wrapper->privAlgorithmDropDown = NULL;
    wrapper->privBlockModeDropDown = NULL;
    wrapper->privKeySizeDropDown = NULL;

    wrapper->data = NULL;

    return wrapper;
};

void asymmetric_key_wrapper_delete(struct asymmetric_key_wrapper * wrapper)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = NULL;

    wrapper->keyPubInputEntry = NULL;
    wrapper->keyPrivInputEntry = NULL;
    wrapper->paramInputEntry = NULL;

    wrapper->keySizeEntry = NULL;

    wrapper->privAlgorithmDropDown = NULL;
    wrapper->privBlockModeDropDown = NULL;
    wrapper->privKeySizeDropDown = NULL;

    wrapper->data = NULL;

    free(wrapper);
}

void asymmetric_key_wrapper_provide_window(struct asymmetric_key_wrapper * wrapper, GtkWidget * mainWindow)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = mainWindow;
}

void asymmetric_key_wrapper_provide_widgets(struct asymmetric_key_wrapper * wrapper, GtkWidget * keyPubInputEntry, GtkWidget * keyPrivInputEntry,
                                       GtkWidget * paramInputEntry, GtkWidget * keySizeEntry, GtkWidget * privAlgorithmDropDown,
                                       GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown)
{
    if(wrapper == NULL)
        return;

    wrapper->keyPubInputEntry = keyPubInputEntry;
    wrapper->keyPrivInputEntry = keyPrivInputEntry;
    wrapper->paramInputEntry = paramInputEntry;

    wrapper->keySizeEntry = keySizeEntry;

    wrapper->privAlgorithmDropDown = privAlgorithmDropDown;
    wrapper->privBlockModeDropDown = privBlockModeDropDown;
    wrapper->privKeySizeDropDown = privKeySizeDropDown;
}

void asymmetric_key_wrapper_provide_data(struct asymmetric_key_wrapper * wrapper, struct asymmetric_key_data * data)
{
    GtkEntryBuffer * entryBuffer = NULL;
    const char * path = NULL;
    int i = 0;

    if(wrapper == NULL || data == NULL)
        return;

    if(wrapper->keyPubInputEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->keyPubInputEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->keyPubPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->keyPubPath, path);
            printf("%s\n", data->keyPubPath);
        }
    }

    if(wrapper->keyPrivInputEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->keyPrivInputEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->keyPrivPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->keyPrivPath, path);
            printf("%s\n", data->keyPrivPath);
        }
    }

    if(wrapper->paramInputEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->paramInputEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            data->paramPath = (char*)malloc(sizeof(char)*(strlen(path)+1));
            strcpy(data->paramPath, path);
            printf("%s\n", data->paramPath);
        }
    }

    if(wrapper->paramInputEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->keySizeEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            i = string_to_int(path);
            if(i >= 512)
                data->keySize = i;
            else
                data->keySize = 512;
        }
        else
            data->keySize = 512;
    }

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
