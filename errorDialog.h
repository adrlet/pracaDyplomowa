#ifndef ERRORDIALOG_H_INCLUDED
#define ERRORDIALOG_H_INCLUDED

#include <gtk/gtk.h>

int error_dialog_init(GtkWidget * mainWindow);

void error_dialog_append_string(char * errorString);

GtkStringList * error_dialog_get_list();

GtkWidget * error_dialog_get_dialog();

#endif // ERRORDIALOG_H_INCLUDED
