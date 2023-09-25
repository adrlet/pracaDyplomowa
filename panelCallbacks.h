#ifndef PANELCALLBACKS_H_INCLUDED
#define PANELCALLBACKS_H_INCLUDED

#include <gtk/gtk.h>

void symInputEntry_cb(GtkEditable * symInputEntry, gpointer user_data);

gboolean symInputFileChooserDialog_close_cb(GtkButton * symInputFileChooserDialog, gpointer user_data);

void symInputFileChooserDialog_cb(GtkDialog * symInputFileChooserDialog, int response, gpointer user_data);

void symInputButton_cb(GtkButton * symInputButton, gpointer user_data);

gboolean symOutputFileChooserDialog_close_cb(GtkButton * symOutputFileChooserDialog, gpointer user_data);

void symOutputFileChooserDialog_cb(GtkDialog * symOutputFileChooserDialog, int response, gpointer user_data);

void symOutputButton_cb(GtkButton * symOutputButton, gpointer user_data);

void symAlgorithmDropDown_cb(GtkDropDown * symAlgorithmDropDown, GParamSpec * pspec, gpointer user_data);

#endif // PANELCALLBACKS_H_INCLUDED
