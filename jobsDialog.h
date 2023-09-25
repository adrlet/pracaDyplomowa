#ifndef JOBSDIALOG_H_INCLUDED
#define SYMMETRICJOBS_H_INCLUDED

#include <gtk/gtk.h>

int jobs_dialog_init(GtkWidget * mainWindow);

void jobs_dialog_append_string(char * errorString);

GtkStringList * jobs_dialog_get_list();

GtkWidget * jobs_dialog_get_dialog();

#endif // SYMMETRICJOBS_H_INCLUDED
