#include "asymmetricParamPanel.h"
#include "asymmetricParamPanel_struct.h"
#include "asymmetricParam.h"
#include "asymmetricParam_struct.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "symmetric_cipher_map.h"
#include "asymmetric_algorithm_map.h"

/// Wczytanie panelu

struct asymmetric_param_wrapper * asymmetric_param_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "asymmetricParam.xml";

    // Uchwyty
    struct asymmetric_param_wrapper * wrapper = NULL;
    struct asymmetric_param_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * asymParamEntry = NULL;
    GtkWidget * asymParamButton = NULL;

    // Dialogi
    GtkWidget * asymParamFileChooserDialog = NULL;
    GFile * curFolder = NULL;

    GtkWidget * spinnerDialog = NULL;
    GtkWidget * alertDialog = NULL;

    // Inne dane
    GtkWidget * asymParamSizeEntry = NULL;

    GtkWidget * asymParamAlgorithmDropDown = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);

    wrapper = asymmetric_param_wrapper_new();
    data = asymmetric_param_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    asymmetric_param_wrapper_provide_window(wrapper, mainWindow);
    asymmetric_param_data_provide_window(data, mainWindow);

    // Dialogi plików
    asymParamFileChooserDialog = gtk_file_chooser_dialog_new("Parametr", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymParamFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    curFolder = g_file_new_for_path(settings_get_settings()->parametersPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymParamFileChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);

    // Dialog spinner
    spinnerDialog = GTK_WIDGET(gtk_builder_get_object(builder, "spinnerDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(spinnerDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(spinnerDialog, "close-request", G_CALLBACK(asymmetric_param_spiner_close_cb), NULL);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    asymParamEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymParamEntry"));;
    asymParamButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymParamButton"));;

    g_signal_connect(asymParamFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymParamEntry);
    g_signal_connect(asymParamButton, "clicked", G_CALLBACK(symOutputButton_cb), asymParamFileChooserDialog);

    asymParamSizeEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymParamSizeEntry"));
    asymParamAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymParamAlgorithmDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymParamAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(asymmetric_get_algorithms())));

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    asymmetric_param_wrapper_provide_widgets(wrapper, asymParamEntry, asymParamSizeEntry, asymParamAlgorithmDropDown);
    asymmetric_param_data_provide_dialogs(data, spinnerDialog, alertDialog);

    return wrapper;
}
