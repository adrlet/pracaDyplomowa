#ifndef ASYMMETRICPARAMPANEL_STRUCT_H_INCLUDED
#define ASYMMETRICPARAMPANEL_STRUCT_H_INCLUDED

#include <gtk/gtk.h>

#include "asymmetricParam_struct.h"

struct asymmetric_param_wrapper
{
    GtkWidget * mainWindow;

    GtkWidget * paramInputEntry;

    GtkWidget * keySizeEntry;

    GtkWidget * algorithmDropDown;

    struct asymmetric_param_data * data;
};

struct asymmetric_param_wrapper * asymmetric_param_wrapper_new();

void asymmetric_param_wrapper_delete(struct asymmetric_param_wrapper * wrapper);

void asymmetric_param_wrapper_provide_window(struct asymmetric_param_wrapper * wrapper, GtkWidget * mainWindow);

void asymmetric_param_wrapper_provide_widgets(struct asymmetric_param_wrapper * wrapper, GtkWidget * paramInputEntry, GtkWidget * paramSizeEntry,
                                       GtkWidget * algorithmDropDown);

void asymmetric_param_wrapper_provide_data(struct asymmetric_param_wrapper * wrapper, struct asymmetric_param_data * data);

#endif // ASYMMETRICPARAMPANEL_STRUCT_H_INCLUDED
