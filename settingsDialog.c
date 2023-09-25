#include <string.h>

#include "settingsDialog.h"

#include "widgetManagment.h"
#include "message_digest_map.h"
#include "bio_pre.h"

#include "errorLog.h"
#include "jobsLog.h"

struct settingsPanel * applicationsettings_dialog = NULL;

const char * settingsDialogFile = "dialogSettings.xml";
const char * settingsDialogID = "settingsDialog";

const char * derive_algorithmValues[] =
{
    "EVP_SHA384", // 0
    "EVP_SHA512_224", // 1
    "EVP_SHA512_256", // 3
    NULL // 4
};

static void settingsDialog_save_cb(GtkDialog * settingsDialog, gint response_id, gpointer user_data)
{
    struct settings * appSettings = NULL;

    if(response_id != GTK_RESPONSE_OK)
        return;

    appSettings = settings_get_settings();
    settings_dialog_provide_data(applicationsettings_dialog, appSettings);
    error_log_set_path(appSettings->errorPath);
    jobs_log_set_path(appSettings->jobsPath);
    setBufferSize(appSettings->bufferSize);
    settings_save_file(appSettings);
}

static void directoryFileChooserDialog_open_cb(GtkDialog * directoryFileChooserDialog, gint response_id, gpointer user_data)
{
    int * state = (int*)user_data;
    GtkWidget * entry = NULL;
    GtkEntryBuffer * entryBuffer = NULL;
    GFile * directory = NULL;
    char * fileName = NULL;

    if(response_id != GTK_RESPONSE_ACCEPT)
        goto end;

    switch(*state)
    {
    case 0:
        entry = applicationsettings_dialog->logsPathEntry;
        break;
    case 1:
        entry = applicationsettings_dialog->keysDirectoryEntry;
        break;
    case 2:
        entry = applicationsettings_dialog->parametersDirectoryEntry;
        break;
    default:
        goto end;
    }

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    directory = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(directoryFileChooserDialog));
    fileName = g_file_get_parse_name(directory);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(entryBuffer), fileName, -1);

end:
    gtk_widget_set_visible(GTK_WIDGET(directoryFileChooserDialog), FALSE);
}

static void pathButton_cb(GtkButton * button, gpointer user_data)
{
    int * state = (int*)user_data;
    GtkWidget * compareButton = GTK_WIDGET(button);

    if(compareButton == applicationsettings_dialog->logsPathButton)
        *state = 0;
    else if(compareButton == applicationsettings_dialog->keysPathButton)
        *state = 1;
    else if(compareButton == applicationsettings_dialog->parametersPathButton)
        *state = 2;

    printf("state %d\n", *state);

    gtk_window_present(GTK_WINDOW(applicationsettings_dialog->directoryFileChooserDialog));
}

void settingsDialog_show_cb(GtkDialog * settingsDialog, gpointer user_data)
{
    settings_provide_data(settings_get_settings(), applicationsettings_dialog);
    gtk_window_present(GTK_WINDOW(settingsDialog));
}

static struct settingsPanel * settings_dialog_new()
{
    struct settingsPanel * panel = NULL;

    panel = (struct settingsPanel*)malloc(sizeof(struct settingsPanel));
    if(panel == NULL)
        return panel;

    panel->settingsDialog = NULL;
    panel->saltLenEntry = NULL;
    panel->bufferSizeEntry = NULL;
    panel->logsPathEntry = NULL;
    panel->keysDirectoryEntry = NULL;
    panel->parametersDirectoryEntry = NULL;
    panel->derive_algorithmDropDown = NULL;

    panel->directoryFileChooserDialog = NULL;
    panel->logsPathButton = NULL;
    panel->keysPathButton = NULL;
    panel->parametersPathButton = NULL;

    return panel;
}

static void settings_dialog_provide_widgets(struct settingsPanel * panel, GtkWidget * settingsDialog, GtkWidget * saltLenEntry,
                                   GtkWidget * bufferSizeEntry, GtkWidget * logsPathEntry, GtkWidget * keysDirectoryEntry,
                                   GtkWidget * parametersDirectoryEntry, GtkWidget * derive_algorithmDropDown, GtkWidget * directoryFileChooserDialog,
                                   GtkWidget * logsPathButton, GtkWidget * keysPathButton, GtkWidget * parametersPathButton)
{
    if(panel == NULL)
        return;

    panel->settingsDialog = settingsDialog;
    panel->saltLenEntry = saltLenEntry;
    panel->bufferSizeEntry = bufferSizeEntry;
    panel->logsPathEntry = logsPathEntry;
    panel->keysDirectoryEntry = keysDirectoryEntry;
    panel->parametersDirectoryEntry = parametersDirectoryEntry;
    panel->derive_algorithmDropDown = derive_algorithmDropDown;

    panel->directoryFileChooserDialog = directoryFileChooserDialog;
    panel->logsPathButton = logsPathButton;
    panel->keysPathButton = keysPathButton;
    panel->parametersPathButton = parametersPathButton;
}

int settings_dialog_init(GtkWidget * mainWindow)
{
    GtkWidget * settingsDialog = NULL;
    GtkWidget * saltLenEntry = NULL;
    GtkWidget * bufferSizeEntry = NULL;
    GtkWidget * logsPathEntry = NULL;
    GtkWidget * keysDirectoryEntry = NULL;
    GtkWidget * parametersDirectoryEntry = NULL;
    GtkWidget * derive_algorithmDropDown = NULL;

    GtkWidget * directoryFileChooserDialog = NULL;
    GtkWidget * logsPathButton = NULL;
    GtkWidget * keysPathButton = NULL;
    GtkWidget * parametersPathButton = NULL;

    GtkStringList * deriveAlgorithmsList = NULL;

    int * state = NULL;;

    GtkBuilder * builder = gtk_builder_new_from_file(settingsDialogFile);
    if(builder == NULL)
        return -1;

    settingsDialog = GTK_WIDGET(gtk_builder_get_object(builder, settingsDialogID));
    saltLenEntry = GTK_WIDGET(gtk_builder_get_object(builder, "settingsSaltEntry"));
    bufferSizeEntry = GTK_WIDGET(gtk_builder_get_object(builder, "settingsBufferSizeEntry"));
    logsPathEntry = GTK_WIDGET(gtk_builder_get_object(builder, "settingsLogsPathEntry"));
    keysDirectoryEntry = GTK_WIDGET(gtk_builder_get_object(builder, "settingsKeysPathEntry"));
    parametersDirectoryEntry = GTK_WIDGET(gtk_builder_get_object(builder, "settingsParametersPathEntry"));
    derive_algorithmDropDown = GTK_WIDGET(gtk_builder_get_object(builder, "settingsDerive_algorithmDropDown"));

    logsPathButton = GTK_WIDGET(gtk_builder_get_object(builder, "settingsLogsPathButton"));
    keysPathButton = GTK_WIDGET(gtk_builder_get_object(builder, "settingsKeysPathButton"));
    parametersPathButton = GTK_WIDGET(gtk_builder_get_object(builder, "settingsParametersPathButton"));

    gtk_window_set_transient_for(GTK_WINDOW(settingsDialog), GTK_WINDOW(mainWindow));
    g_object_unref(builder);

    deriveAlgorithmsList = gtk_string_list_new(derive_algorithmValues);
    gtk_drop_down_set_model(GTK_DROP_DOWN(derive_algorithmDropDown), G_LIST_MODEL(deriveAlgorithmsList));

    directoryFileChooserDialog = gtk_file_chooser_dialog_new("Katalog", GTK_WINDOW(settingsDialog), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                             "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    applicationsettings_dialog = settings_dialog_new();
    settings_dialog_provide_widgets(applicationsettings_dialog, settingsDialog, saltLenEntry, bufferSizeEntry, logsPathEntry,
                                  keysDirectoryEntry, parametersDirectoryEntry, derive_algorithmDropDown, directoryFileChooserDialog,
                                  logsPathButton, keysPathButton, parametersPathButton);

    state = (int*)malloc(sizeof(int));
    g_signal_connect_swapped(directoryFileChooserDialog, "close-request", G_CALLBACK(gtk_widget_hide), directoryFileChooserDialog);
    g_signal_connect(directoryFileChooserDialog, "response", G_CALLBACK(directoryFileChooserDialog_open_cb), state);

    g_signal_connect(logsPathButton, "clicked", G_CALLBACK(pathButton_cb), state);
    g_signal_connect(keysPathButton, "clicked", G_CALLBACK(pathButton_cb), state);
    g_signal_connect(parametersPathButton, "clicked", G_CALLBACK(pathButton_cb), state);

    g_signal_connect(settingsDialog, "response", G_CALLBACK(settingsDialog_save_cb), settings_get_settings());

    return 0;
}

void settings_dialog_provide_data(struct settingsPanel * panel, struct settings * appSettings)
{
    const char * buffer = NULL;
    char * buffer2 = NULL;
    GtkEntryBuffer * entryBuffer = NULL;
    int value = 0;
    const char * errorLogName = NULL;
    const char * jobsLogName = NULL;

    if(panel == NULL || appSettings == NULL)
        return;

    errorLogName = settings_get_error_log_name();
    jobsLogName = settings_get_jobs_log_name();

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->saltLenEntry));
    buffer = gtk_entry_buffer_get_text(entryBuffer);
    value = string_to_int(buffer);
    if(value > 0)
        appSettings->saltLen = value;

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->bufferSizeEntry));
    buffer = gtk_entry_buffer_get_text(entryBuffer);
    value = string_to_int(buffer);
    if(value >= DEFAULT_BUFFER_SIZE)
        appSettings->bufferSize = value;

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->logsPathEntry));
    buffer = gtk_entry_buffer_get_text(entryBuffer);
    if(buffer != NULL)
    {
        if(appSettings->errorPath != NULL)
            free(appSettings->errorPath);

        if((strlen(buffer)+strlen(errorLogName)) < MAX_PATH_LEN)
        {
            buffer2 = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
            strcpy(buffer2, buffer);
            strcat(buffer2, errorLogName);
            appSettings->errorPath = buffer2;
        }

        if(appSettings->jobsPath != NULL)
            free(appSettings->jobsPath);

        if((strlen(buffer)+strlen(jobsLogName)) < MAX_PATH_LEN)
        {
            buffer2 = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
            strcpy(buffer2, buffer);
            strcat(buffer2, jobsLogName);
            appSettings->jobsPath = buffer2;
        }
    }

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->keysDirectoryEntry));
    buffer = gtk_entry_buffer_get_text(entryBuffer);
    if(buffer != NULL)
    {
        if(appSettings->keysPath != NULL)
            free(appSettings->keysPath);

        if(strlen(buffer) < MAX_PATH_LEN)
        {
            buffer2 = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
            strcpy(buffer2, buffer);
            appSettings->keysPath = buffer2;
        }
    }

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->parametersDirectoryEntry));
    buffer = gtk_entry_buffer_get_text(entryBuffer);
    if(buffer != NULL)
    {
        if(appSettings->parametersPath != NULL)
            free(appSettings->parametersPath);

        if(strlen(buffer) < MAX_PATH_LEN)
        {
            buffer2 = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
            strcpy(buffer2, buffer);
            appSettings->parametersPath = buffer2;
        }
    }

    value = gtk_drop_down_get_selected(GTK_DROP_DOWN(panel->derive_algorithmDropDown));
    if(value < MESSAGE_DIGEST_ALGORITHMS && value >= 0)
        appSettings->derive_algorithm = value;
}

GtkWidget * settings_dialog_get_dialog()
{
    return applicationsettings_dialog->settingsDialog;
}

const char ** settings_dialog_get_derive_algorithmValues()
{
    return derive_algorithmValues;
}
