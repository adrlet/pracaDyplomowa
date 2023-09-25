#ifndef ASYMMETRICSIGNPANEL_STRUCT_H_INCLUDED
#define ASYMMETRICSIGNPPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#include "asymmetricSign_struct.h"

struct asymmetric_sign_wrapper
{
    GtkWidget * mainWindow; // 8

    GtkWidget * inputPathEntry; // 16
    GtkWidget * outputPathEntry; // 24
    GtkWidget * keyPathEntry; // 32

    GtkWidget * digestDropDown; // 40
    GtkWidget * base64Check;

    GtkWidget * privAlgorithmDropDown; // 56
    GtkWidget * privBlockModeDropDown; // 64
    GtkWidget * privKeySizeDropDown; // 72

    struct asymmetric_sign_data * data; // 80
};

struct asymmetric_sign_wrapper * asymmetric_sign_wrapper_new();

void asymmetric_sign_wrapper_delete(struct asymmetric_sign_wrapper * wrapper);

void asymmetric_sign_wrapper_provide_window(struct asymmetric_sign_wrapper * wrapper, GtkWidget * mainWindow);

void asymmetric_sign_wrapper_provide_widgets(struct asymmetric_sign_wrapper * wrapper, GtkWidget * inputPathEntry,
                                             GtkWidget * outputPathEntry, GtkWidget * keyPathEntry,
                                             GtkWidget * digestDropDown, GtkWidget * base64Check,
                                             GtkWidget * privAlgorithmDropDown, GtkWidget * privBlockModeDropDown, GtkWidget * privKeySizeDropDown);

void asymmetric_sign_wrapper_provide_data(struct asymmetric_sign_wrapper * wrapper, struct asymmetric_sign_data * data);

#endif // ASYMMETRICSIGNPPANEL_STRUCT_H_INCLUDED
