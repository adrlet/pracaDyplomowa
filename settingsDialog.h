#ifndef SYMMETRICSETTINGS_H_INCLUDED
#define SYMMETRICSETTINGS_H_INCLUDED

#include <gtk/gtk.h>

#include "settings.h"

struct settingsPanel
{
    GtkWidget * settingsDialog; // 8
    GtkWidget * saltLenEntry; // 16
    GtkWidget * bufferSizeEntry; // 24
    GtkWidget * logsPathEntry; // 32
    GtkWidget * keysDirectoryEntry; // 40
    GtkWidget * parametersDirectoryEntry; // 48
    GtkWidget * derive_algorithmDropDown; // 56

    GtkWidget * directoryFileChooserDialog; // 64
    GtkWidget * logsPathButton; // 72
    GtkWidget * keysPathButton; // 80
    GtkWidget * parametersPathButton; // 88
};

void settings_dialog_provide_data(struct settingsPanel * panel, struct settings * appSettings);

GtkWidget * settingsDialog_open_cb();

void settingsDialog_show_cb(GtkDialog * self, gpointer user_data);

int settings_dialog_init(GtkWidget * mainWindow);

GtkWidget * settings_dialog_get_dialog();

const char ** settings_dialog_get_derive_algorithmValues();

#endif // SYMMETRICSETTINGS_H_INCLUDED
