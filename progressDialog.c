#include "progressDialog.h"

GtkWidget * progressDialog = NULL;
const char * progressDialogFile = "dialogProgress.xml";
const char * progressDialogID = "progressDialog";
const char * progressTemplateId = "progressBox";

int progress_dialog_init(GtkWidget * mainWindow)
{
    GtkBuilder * builder = gtk_builder_new_from_file(progressDialogFile);
    if(builder == NULL)
        return -1;

    progressDialog = GTK_WIDGET(gtk_builder_get_object(builder, progressDialogID));
    gtk_window_set_transient_for(GTK_WINDOW(progressDialog), GTK_WINDOW(mainWindow));
    g_object_unref(builder);

    return 0;
}

GtkWidget * progress_dialog_get_dialog()
{
    return progressDialog;
}

GtkWidget * progress_dialog_get_template()
{
    GtkWidget * templateWidget = NULL;
    GtkBuilder * builder = gtk_builder_new_from_file(progressDialogFile);
    if(builder == NULL)
        return NULL;

    templateWidget = GTK_WIDGET(gtk_builder_get_object(builder, progressTemplateId));
    g_object_ref(templateWidget);
    g_object_unref(builder);

    return templateWidget;
}
