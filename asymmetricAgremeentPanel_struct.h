#ifndef ASYMMETRICAGREMEENTPANEL_STRUCT_H_INCLUDED
#define ASYMMETRICAGREMEENTPPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#include "asymmetricAgremeent_struct.h"

struct asymmetric_agremeent_wrapper
{
    GtkWidget * mainWindow; // 8

    GtkWidget * keyPathEntry; // 16
    GtkWidget * privKeyPathEntry; // 24
    GtkWidget * pubKeyPathEntry; // 24
    GtkWidget * outputPathEntry; // 32

    GtkWidget * base64Check;

    GtkWidget * privAlgorithmDropDown; // 56
    GtkWidget * privBlockModeDropDown; // 64
    GtkWidget * privKeySizeDropDown; // 72

    struct asymmetric_agremeent_data * data; // 40
};

struct asymmetric_agremeent_wrapper * asymmetric_agremeent_wrapper_new();

void asymmetric_agremeent_wrapper_delete(struct asymmetric_agremeent_wrapper * wrapper);

void asymmetric_agremeent_wrapper_provide_window(struct asymmetric_agremeent_wrapper * wrapper, GtkWidget * mainWindow);

void asymmetric_agremeent_wrapper_provide_widgets(struct asymmetric_agremeent_wrapper * wrapper, GtkWidget * keyPathEntry,
                                             GtkWidget * privKeyPathEntry, GtkWidget * pubKeyPathEntry, GtkWidget * outputPathEntry,
                                             GtkWidget * base64Check, GtkWidget * privAlgorithmDropDown,
                                             GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown);

void asymmetric_agremeent_wrapper_provide_data(struct asymmetric_agremeent_wrapper * wrapper, struct asymmetric_agremeent_data * data);

#endif // ASYMMETRICAGREMEENTPPANEL_STRUCT_H_INCLUDED
