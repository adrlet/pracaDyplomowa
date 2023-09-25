#include "asymmetricAgremeentPanel.h"
#include "asymmetricAgremeentPanel_struct.h"
#include "asymmetricAgremeent.h"
#include "asymmetricAgremeent_struct.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "message_digest_map.h"
#include "asymmetric_algorithm_map.h"
#include "symmetric_cipher_map.h"

/// Wczytanie panelu

struct asymmetric_agremeent_wrapper * asymmetric_agremeent_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "asymmetricAgremeent.xml";

    // Uchwyty
    struct asymmetric_agremeent_wrapper * wrapper = NULL;
    struct asymmetric_agremeent_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * asymAgreeKeyPeerEntry = NULL;
    GtkWidget * asymAgreeKeyPeerButton = NULL;
    GtkWidget * asymAgreeKeyPrivEntry = NULL;
    GtkWidget * asymAgreeKeyPrivButton = NULL;
    GtkWidget * asymAgreeKeyPubEntry = NULL;
    GtkWidget * asymAgreeKeyPubButton = NULL;
    GtkWidget * asymAgreeOutputEntry = NULL;
    GtkWidget * asymAgreeOutputButton = NULL;

    // Dialogi
    GtkWidget * asymAgreeKeyPeerFileChooserDialog = NULL;
    GtkWidget * asymAgreeKeyPrivFileChooserDialog = NULL;
    GtkWidget * asymAgreeKeyPubFileChooserDialog = NULL;
    GtkWidget * asymAgreeOutputFileChooserDialog = NULL;
    GFile * curFolder = NULL;

    GtkWidget * spinnerDialog = NULL;
    GtkWidget * alertDialog = NULL;
    GtkWidget * passwordDialog = NULL;

    // Inne dane
    GtkWidget * asymKeyPrivAlgorithmDropDown = NULL;
    GtkWidget * asymKeyPrivBlockModeDropDown = NULL;
    GtkWidget * asymKeyPrivKeySizeDropDown = NULL;

    GtkWidget * asymBase64Check = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogPassword.xml", NULL);

    wrapper = asymmetric_agremeent_wrapper_new();
    data = asymmetric_agremeent_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    asymmetric_agremeent_wrapper_provide_window(wrapper, mainWindow);
    asymmetric_agremeent_data_provide_window(data, mainWindow);

    asymAgreeKeyPeerFileChooserDialog = gtk_file_chooser_dialog_new("Klucz", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymAgreeKeyPeerFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymAgreeKeyPrivFileChooserDialog = gtk_file_chooser_dialog_new("Parametr", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymAgreeKeyPrivFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymAgreeKeyPubFileChooserDialog = gtk_file_chooser_dialog_new("Parametr", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymAgreeKeyPubFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymAgreeOutputFileChooserDialog = gtk_file_chooser_dialog_new("Wyjście", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymAgreeOutputFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    curFolder = g_file_new_for_path(settings_get_settings()->keysPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymAgreeKeyPeerFileChooserDialog), curFolder, NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymAgreeKeyPrivFileChooserDialog), curFolder, NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymAgreeKeyPubFileChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Dialog spinner
    spinnerDialog = GTK_WIDGET(gtk_builder_get_object(builder, "spinnerDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(spinnerDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(spinnerDialog, "close-request", G_CALLBACK(asymmetric_agremeent_spiner_close_cb), NULL);

    // Dialog password
    passwordDialog = GTK_WIDGET(gtk_builder_get_object(builder, "passwordDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(passwordDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(passwordDialog, "response", G_CALLBACK(asymmetric_agremeent_password_cb), data);
    g_signal_connect(passwordDialog, "close-request", G_CALLBACK(asymmetric_agremeent_password_close_cb), data);

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    asymAgreeKeyPeerEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPeerEntry"));
    asymAgreeKeyPeerButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPeerButton"));
    asymAgreeKeyPrivEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPrivEntry"));
    asymAgreeKeyPrivButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPrivButton"));
    asymAgreeKeyPubEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPubEntry"));
    asymAgreeKeyPubButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeKeyPubButton"));
    asymAgreeOutputEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeOutputEntry"));
    asymAgreeOutputButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymAgreeOutputButton"));

    g_signal_connect(asymAgreeKeyPeerFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymAgreeKeyPeerEntry);
    g_signal_connect(asymAgreeKeyPeerButton, "clicked", G_CALLBACK(symOutputButton_cb), asymAgreeKeyPeerFileChooserDialog);
    g_signal_connect(asymAgreeKeyPrivFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymAgreeKeyPrivEntry);
    g_signal_connect(asymAgreeKeyPrivButton, "clicked", G_CALLBACK(symOutputButton_cb), asymAgreeKeyPrivFileChooserDialog);
    g_signal_connect(asymAgreeKeyPubFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymAgreeKeyPubEntry);
    g_signal_connect(asymAgreeKeyPubButton, "clicked", G_CALLBACK(symOutputButton_cb), asymAgreeKeyPubFileChooserDialog);
    g_signal_connect(asymAgreeOutputFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymAgreeOutputEntry);
    g_signal_connect(asymAgreeOutputButton, "clicked", G_CALLBACK(symOutputButton_cb), asymAgreeOutputFileChooserDialog);

    asymBase64Check = GTK_WIDGET(gtk_builder_get_object(builder, "asymBase64Check"));

    asymKeyPrivAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivAlgorithmDropDown"));
    asymKeyPrivBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivBlockModeDropDown"));
    asymKeyPrivKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(asymKeyPrivAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    //asymmetric_agremeent_wrapper_provide_widgets(wrapper, asymAgreeKeyEntry, asymAgreeParamEntry, asymAgreeOutputEntry, asymBase64Check);
    asymmetric_agremeent_wrapper_provide_widgets(wrapper, asymAgreeKeyPeerEntry, asymAgreeKeyPrivEntry, asymAgreeKeyPubEntry,
                                                 asymAgreeOutputEntry, asymBase64Check, asymKeyPrivAlgorithmDropDown, asymKeyPrivBlockModeDropDown,
                                                 asymKeyPrivKeySizeDropDown);
    asymmetric_agremeent_data_provide_dialogs(data, spinnerDialog, alertDialog, passwordDialog);

    return wrapper;
}
