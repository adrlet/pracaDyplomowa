#include <string.h>

#include "jobsDialog.h"
#include "widgetManagment.h"
#include "log.h"

GtkWidget * jobsDialog = NULL;
GtkStringList * jobsStringList = NULL;
const char * jobsDialogFile = "dialogJobs.xml";
const char * jobsDialogID = "jobsDialog";

G_MODULE_EXPORT char * get_jobs_date_time(GtkStringObject * record)
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

G_MODULE_EXPORT char * get_jobs_date_time_factory(GtkListItem *item, GtkStringObject * record)
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

G_MODULE_EXPORT char * get_jobs_operation(GtkStringObject * record)
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

G_MODULE_EXPORT char * get_jobs_operation_factory(GtkListItem *item, GtkStringObject * record)
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

G_MODULE_EXPORT char * get_jobs_result(GtkStringObject * record)
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

G_MODULE_EXPORT char * get_jobs_result_factory(GtkListItem *item, GtkStringObject * record)
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

void jobs_dialog_append_string(char * jobsString)
{
    printf("jobsString: %s\n", jobsString);
    gtk_string_list_append(jobsStringList, jobsString);
}

GtkWidget * jobs_dialog_get_dialog()
{
    return jobsDialog;
}

GtkStringList * jobs_dialog_get_list()
{
    return jobsStringList;
}

int jobs_dialog_init(GtkWidget * mainWindow)
{
    GtkSortListModel * sortListModel = NULL;

    GtkBuilder * builder = gtk_builder_new_from_file(jobsDialogFile);
    if(builder == NULL)
        return -1;

    jobsDialog = GTK_WIDGET(gtk_builder_get_object(builder, jobsDialogID));

    jobsStringList = gtk_string_list_new(NULL);
    sortListModel = GTK_SORT_LIST_MODEL(gtk_builder_get_object(builder, "sortlist"));
    gtk_sort_list_model_set_model(sortListModel, G_LIST_MODEL(jobsStringList));

    gtk_window_set_transient_for(GTK_WINDOW(jobsDialog), GTK_WINDOW(mainWindow));
    g_object_unref(builder);

    return 0;
}
