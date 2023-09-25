#include "asymmetricKeyPanel.h"
#include "asymmetricKeyPanel_struct.h"
#include "asymmetricKey.h"
#include "asymmetricKey_struct.h"
#include "symmetricDeriveKey.h"
#include "symmetricDeriveKey_struct.h"

#include "widgetManagment.h"
#include "settingsDialog.h"
#include "panelCallbacks.h"
#include "symmetric_cipher_map.h"

/// Wczytanie panelu

struct asymmetric_key_wrapper * asymmetric_key_create_panel(GtkWidget * functionSectionBox)
{
    // UI
    GtkBuilder * builder = NULL;
    static const char * functionsGroupFile = "asymmetricKey.xml";

    // Uchwyty
    struct asymmetric_key_wrapper * wrapper = NULL;
    struct asymmetric_key_data * data = NULL;

    // Elementy przetwarzane
    GtkWidget * mainWindow = NULL;

    GtkWidget * symFunctionDataBox = NULL;
    GtkWidget * asymKeyPubEntry = NULL;
    GtkWidget * asymKeyPubButton = NULL;
    GtkWidget * asymKeyPrivEntry = NULL;
    GtkWidget * asymKeyPrivButton = NULL;
    GtkWidget * asymKeyParamEntry = NULL;
    GtkWidget * asymKeyParamButton = NULL;

    // Dialogi
    GtkWidget * asymKeyPubFileChooserDialog = NULL;
    GtkWidget * asymKeyPrivFileChooserDialog = NULL;
    GtkWidget * asymKeyParamFileChooserDialog = NULL;
    GFile * curFolder = NULL;

    GtkWidget * spinnerDialog = NULL;
    GtkWidget * alertDialog = NULL;
    GtkWidget * passwordDialog = NULL;

    // Inne dane
    GtkWidget * asymKeySizeEntry = NULL;

    GtkWidget * asymKeyPrivAlgorithmDropDown = NULL;
    GtkWidget * asymKeyPrivBlockModeDropDown = NULL;
    GtkWidget * asymKeyPrivKeySizeDropDown = NULL;

    // Alokacja danych
    builder = gtk_builder_new_from_file(functionsGroupFile);
    if(builder == NULL)
        return NULL;

    gtk_builder_add_from_file(builder, "dialogPassword.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogSpinner.xml", NULL);
    gtk_builder_add_from_file(builder, "dialogAlert.xml", NULL);

    wrapper = asymmetric_key_wrapper_new();
    data = asymmetric_key_data_new();

    wrapper->data = data;

    mainWindow = get_parent_of_type(functionSectionBox, GTK_TYPE_APPLICATION_WINDOW);

    asymmetric_key_wrapper_provide_window(wrapper, mainWindow);
    asymmetric_key_data_provide_window(data, mainWindow);

    // Dialogi plików
    asymKeyPubFileChooserDialog = gtk_file_chooser_dialog_new("Klucz publiczny", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymKeyPubFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymKeyPrivFileChooserDialog = gtk_file_chooser_dialog_new("Klucz prywatny", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Save", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymKeyPrivFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    asymKeyParamFileChooserDialog = gtk_file_chooser_dialog_new("Parametr", GTK_WINDOW(mainWindow),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "_Cancel", GTK_RESPONSE_CANCEL,
                                                        "_Open", GTK_RESPONSE_ACCEPT,
                                                        NULL);
    g_signal_connect(asymKeyParamFileChooserDialog, "close-request", G_CALLBACK(symOutputFileChooserDialog_close_cb), NULL);

    curFolder = g_file_new_for_path(settings_get_settings()->keysPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymKeyPubFileChooserDialog), curFolder, NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymKeyPrivFileChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);
    curFolder = g_file_new_for_path(settings_get_settings()->parametersPath);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(asymKeyParamFileChooserDialog), curFolder, NULL);
    g_object_unref(curFolder);

    // Dialog spinner
    spinnerDialog = GTK_WIDGET(gtk_builder_get_object(builder, "spinnerDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(spinnerDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(spinnerDialog, "close-request", G_CALLBACK(symmetric_derive_key_spiner_close_cb), NULL);

    // Dialog alert
    alertDialog = GTK_WIDGET(gtk_builder_get_object(builder, "alertDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(alertDialog), GTK_WINDOW(mainWindow));

    // Dialog password
    passwordDialog = GTK_WIDGET(gtk_builder_get_object(builder, "passwordDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(passwordDialog), GTK_WINDOW(mainWindow));
    g_signal_connect(passwordDialog, "response", G_CALLBACK(asymmetric_key_password_cb), data);
    g_signal_connect(passwordDialog, "close-request", G_CALLBACK(asymmetric_key_password_close_cb), data);

    // Przetwarzanie panelu
    symFunctionDataBox = GTK_WIDGET(gtk_builder_get_object(builder, "functionDataBox"));

    asymKeyPubEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPubEntry"));;
    asymKeyPubButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPubButton"));;
    asymKeyPrivEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivEntry"));;
    asymKeyPrivButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivButton"));;
    asymKeyParamEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyParamEntry"));;
    asymKeyParamButton = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyParamButton"));;

    g_signal_connect(asymKeyPubFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymKeyPubEntry);
    g_signal_connect(asymKeyPubButton, "clicked", G_CALLBACK(symInputButton_cb), asymKeyPubFileChooserDialog);
    g_signal_connect(asymKeyPrivFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymKeyPrivEntry);
    g_signal_connect(asymKeyPrivButton, "clicked", G_CALLBACK(symOutputButton_cb), asymKeyPrivFileChooserDialog);
    g_signal_connect(asymKeyParamFileChooserDialog, "response", G_CALLBACK(symOutputFileChooserDialog_cb), asymKeyParamEntry);
    g_signal_connect(asymKeyParamButton, "clicked", G_CALLBACK(symOutputButton_cb), asymKeyParamFileChooserDialog);

    asymKeySizeEntry = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeySizeEntry"));
    asymKeyPrivAlgorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivAlgorithmDropDown"));
    asymKeyPrivBlockModeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivBlockModeDropDown"));
    asymKeyPrivKeySizeDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "asymKeyPrivKeySizeDropDown"));

    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivAlgorithmDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_algorithms())));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivBlockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[0])));
    gtk_drop_down_set_model(GTK_DROP_DOWN(asymKeyPrivKeySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[0])));
    g_signal_connect(asymKeyPrivAlgorithmDropDown, "notify::selected-item", G_CALLBACK(symAlgorithmDropDown_cb), NULL);

    gtk_box_append(GTK_BOX(functionSectionBox), symFunctionDataBox);

    // Ustawienie uchwytow
    asymmetric_key_wrapper_provide_widgets(wrapper, asymKeyPubEntry, asymKeyPrivEntry, asymKeyParamEntry, asymKeySizeEntry,
                                           asymKeyPrivAlgorithmDropDown, asymKeyPrivBlockModeDropDown, asymKeyPrivKeySizeDropDown);
    asymmetric_key_data_provide_dialogs(data, spinnerDialog, alertDialog, passwordDialog);

    return wrapper;
}
