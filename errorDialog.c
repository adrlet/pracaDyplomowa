#include <string.h>

#include "errorDialog.h"
#include "widgetManagment.h"
#include "log.h"

GtkWidget * errorDialog = NULL;
GtkStringList * errorStringList = NULL;
const char * errorDialogFile = "dialogError.xml";
const char * errorDialogID = "errorDialog";

G_MODULE_EXPORT char * get_error_date_time(GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 0, separator);
    length = skipSeparator(stringRecord, 1, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_date_time_factory(GtkListItem *item, GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 0, separator);
    length = skipSeparator(stringRecord, 1, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_function(GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 1, separator);
    length = skipSeparator(stringRecord, 2, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_function_factory(GtkListItem *item, GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 1, separator);
    length = skipSeparator(stringRecord, 2, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_type(GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 2, separator);
    length = skipSeparator(stringRecord, 3, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_type_factory(GtkListItem *item, GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 2, separator);
    length = skipSeparator(stringRecord, 3, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_desc(GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 3, separator);
    length = skipSeparator(stringRecord, 4, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_desc_factory(GtkListItem *item, GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 3, separator);
    length = skipSeparator(stringRecord, 4, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_openssl(GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 4, separator);
    length = skipSeparator(stringRecord, 5, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

G_MODULE_EXPORT char * get_error_openssl_factory(GtkListItem *item, GtkStringObject * record)
{
    int position = 0;
    int length = 0;
    char * buffer = NULL;
    const char * stringRecord = NULL;

    if(record == NULL)
        return NULL;

    stringRecord = gtk_string_object_get_string(record);

    position = skipSeparator(stringRecord, 4, separator);
    length = skipSeparator(stringRecord, 5, separator)-position;

    buffer = (char*)malloc(sizeof(char)*(length+1));
    strncpy(buffer, stringRecord+position, length);
    buffer[length-1] = 0;

    return buffer;
}

void error_dialog_append_string(char * errorString)
{
    printf("errorString: %s\n", errorString);
    gtk_string_list_append(errorStringList, errorString);
}

GtkWidget * error_dialog_get_dialog()
{
    return errorDialog;
}

GtkStringList * error_dialog_get_list()
{
    return errorStringList;
}

int error_dialog_init(GtkWidget * mainWindow)
{
    GtkSortListModel * sortListModel = NULL;

    GtkBuilder * builder = gtk_builder_new_from_file(errorDialogFile);
    if(builder == NULL)
        return -1;

    errorDialog = GTK_WIDGET(gtk_builder_get_object(builder, errorDialogID));

    errorStringList = gtk_string_list_new(NULL);
    sortListModel = GTK_SORT_LIST_MODEL(gtk_builder_get_object(builder, "sortlist"));
    gtk_sort_list_model_set_model(sortListModel, G_LIST_MODEL(errorStringList));

    gtk_window_set_transient_for(GTK_WINDOW(errorDialog), GTK_WINDOW(mainWindow));
    g_object_unref(builder);

    return 0;
}
