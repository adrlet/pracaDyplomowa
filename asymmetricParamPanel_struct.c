#include <string.h>

#include "asymmetricParamPanel_struct.h"
#include "widgetManagment.h"
#include "symmetric_cipher_map.h"

struct asymmetric_param_wrapper * asymmetric_param_wrapper_new()
{
    struct asymmetric_param_wrapper * wrapper = NULL;

    wrapper = (struct asymmetric_param_wrapper*)malloc(sizeof(struct asymmetric_param_wrapper));
    if(wrapper == NULL)
        return wrapper;

    wrapper->mainWindow = NULL;

    wrapper->paramInputEntry = NULL;

    wrapper->keySizeEntry = NULL;

    wrapper->algorithmDropDown = NULL;

    wrapper->data = NULL;

    return wrapper;
};

void asymmetric_param_wrapper_delete(struct asymmetric_param_wrapper * wrapper)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = NULL;

    wrapper->paramInputEntry = NULL;

    wrapper->keySizeEntry = NULL;

    wrapper->algorithmDropDown = NULL;

    wrapper->data = NULL;

    free(wrapper);
}

void asymmetric_param_wrapper_provide_window(struct asymmetric_param_wrapper * wrapper, GtkWidget * mainWindow)
{
    if(wrapper == NULL)
        return;

    wrapper->mainWindow = mainWindow;
}

void asymmetric_param_wrapper_provide_widgets(struct asymmetric_param_wrapper * wrapper, GtkWidget * paramInputEntry, GtkWidget * paramSizeEntry,
                                       GtkWidget * algorithmDropDown)
{
    if(wrapper == NULL)
        return;

    wrapper->paramInputEntry = paramInputEntry;

    wrapper->keySizeEntry = paramSizeEntry;

    wrapper->algorithmDropDown = algorithmDropDown;
}

void asymmetric_param_wrapper_provide_data(struct asymmetric_param_wrapper * wrapper, struct asymmetric_param_data * data)
{
    GtkEntryBuffer * entryBuffer = NULL;
    const char * path = NULL;
    int i = 0;

    if(wrapper == NULL || data == NULL)
        return;

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

    if(wrapper->algorithmDropDown != NULL)
        data->algorithm = gtk_drop_down_get_selected(GTK_DROP_DOWN(wrapper->algorithmDropDown));

    if(wrapper->keySizeEntry != NULL)
    {
        entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(wrapper->keySizeEntry));
        path = gtk_entry_buffer_get_text(entryBuffer);
        if(path != NULL && path[0] != 0)
        {
            i = string_to_int(path);
            if(i < 512)
                data->keySize = i;
            else
                data->keySize = 512;
        }
        else
            data->keySize = 512;
    }
}
