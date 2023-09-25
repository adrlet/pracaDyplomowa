#ifndef PROGRESSDIALOG_H_INCLUDED
#define PROGRESSDIALOG_H_INCLUDED

#include <gtk/gtk.h>

int progress_dialog_init(GtkWidget * mainWindow);

GtkWidget * progress_dialog_get_dialog();

GtkWidget * progress_dialog_get_template();

#endif // PROGRESSDIALOG_H_INCLUDED
