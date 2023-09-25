#include "panelCallbacks.h"
#include "widgetManagment.h"
#include "symmetric_cipher_map.h"

// Wyswietlenie danych pliku wejsciowego
G_MODULE_EXPORT void symInputEntry_cb(GtkEditable * symInputEntry, gpointer user_data)
{
    // Zmienne pomocnicze
    char * inputEntry_cbBuffer = NULL;
    unsigned long long fileSize = 0;
    const char * filename = NULL;
    char * modificationtime = NULL;

    // Wejsciowe widgety
    GtkWidget * inputDescriptionBox = NULL;
    GtkEntryBuffer * entryBuffer = NULL;

    // Inicjalizacja
    inputEntry_cbBuffer = (char*)malloc(sizeof(char)*128);

    // Deskryptory pliku
    GFile * file = NULL;
    GFileInfo * fileInfo = NULL;
    GIcon * fileIcon = NULL;
    GDateTime * fileDateTime = NULL;

    // Widgety opisujace
    GtkWidget * inputIcon = NULL;
    GtkWidget * inputFileName = NULL;
    GtkWidget * inputFileSize = NULL;
    GtkWidget * inputFileMod = NULL;

    inputDescriptionBox = GTK_WIDGET(user_data);
    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(symInputEntry));

    // Ustawienie deskryptorow
    filename = gtk_entry_buffer_get_text(entryBuffer);
    file = g_file_new_for_path(filename);
    fileInfo = g_file_query_info(file, "standard::*,time::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if(fileInfo == NULL)
        goto end;
    fileIcon = g_file_info_get_icon(fileInfo);

    // Wyswietlenie ikonki
    inputIcon = get_child_of_type(inputDescriptionBox, GTK_TYPE_IMAGE, 0);
    gtk_image_set_from_gicon(GTK_IMAGE(inputIcon), fileIcon);

    // Wyswietlenie nazwy pliku
    sprintf(inputEntry_cbBuffer, "Name: %s", g_file_info_get_display_name(fileInfo));
    inputFileName = get_child_of_type(inputIcon, GTK_TYPE_LABEL, 1);
    gtk_label_set_text(GTK_LABEL(inputFileName), inputEntry_cbBuffer);

    // Wyswietlenie rozmiaru pliku
    fileSize = (unsigned long long)g_file_info_get_size(fileInfo);
    humanReadableBytes(fileSize, inputEntry_cbBuffer);
    inputFileSize = get_child_of_type(inputFileName, GTK_TYPE_LABEL, 1);
    gtk_label_set_text(GTK_LABEL(inputFileSize), inputEntry_cbBuffer);

    // Wyswietlenie czasu modyfikacji
    fileDateTime = g_file_info_get_modification_date_time(fileInfo);
    modificationtime = g_date_time_format(fileDateTime, "%d/%m/%Y %H:%M:%S");
    sprintf(inputEntry_cbBuffer, "Changed: %s", modificationtime);
    inputFileMod = get_child_of_type(inputFileSize, GTK_TYPE_LABEL, 1);
    gtk_label_set_text(GTK_LABEL(inputFileMod), inputEntry_cbBuffer);

end:
    // Wyczyszczenie danych
    if(file != NULL)
        g_object_unref(file);
    if(fileInfo)
        g_object_unref(fileInfo);
    if(inputEntry_cbBuffer != NULL)
        free(inputEntry_cbBuffer);
    if(modificationtime != NULL)
        g_free(modificationtime);
}

// Schowanie przy zamknięciu
gboolean symInputFileChooserDialog_close_cb(GtkButton * symInputFileChooserDialog, gpointer user_data)
{
    gtk_widget_set_visible(GTK_WIDGET(symInputFileChooserDialog), FALSE);
    return TRUE;
}

// Odczyt pliku odczytu
void symInputFileChooserDialog_cb(GtkDialog * symInputFileChooserDialog, int response, gpointer user_data)
{
    GtkWidget * symInputEntry = GTK_WIDGET(user_data);
    GtkEntryBuffer * symInputEntryBuffer = NULL;

    GFile * openFile = NULL;
    char * fullFileName = NULL;

    // pobierz sciezke wybranego pliku
    if(response == GTK_RESPONSE_ACCEPT)
    {
        symInputEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(symInputEntry));
        openFile = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(symInputFileChooserDialog));
        fullFileName = g_file_get_parse_name(openFile);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(symInputEntryBuffer), fullFileName, -1);
    }

    gtk_widget_set_visible(GTK_WIDGET(symInputFileChooserDialog), FALSE);
}

// Otwarcie dialogu odczytu
void symInputButton_cb(GtkButton * symInputButton, gpointer user_data)
{
    GtkWidget * symInputFileChooserDialog = GTK_WIDGET(user_data);
    gtk_window_present(GTK_WINDOW(symInputFileChooserDialog));
}

// Schowanie przy zamknięciu
gboolean symOutputFileChooserDialog_close_cb(GtkButton * symOutputFileChooserDialog, gpointer user_data)
{
    gtk_widget_set_visible(GTK_WIDGET(symOutputFileChooserDialog), FALSE);
    return TRUE;
}

// Odczyt pliku zapisu
void symOutputFileChooserDialog_cb(GtkDialog * symOutputFileChooserDialog, int response, gpointer user_data)
{
    GtkWidget * symOutputEntry = GTK_WIDGET(user_data);
    GtkEntryBuffer * symOutputEntryBuffer = NULL;

    GFile * saveFile = NULL;
    char * fullFileName = NULL;

    // pobierz sciezke wybranego pliku
    if(response == GTK_RESPONSE_ACCEPT)
    {
        symOutputEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(symOutputEntry));
        saveFile = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(symOutputFileChooserDialog));
        fullFileName = g_file_get_parse_name(saveFile);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(symOutputEntryBuffer), fullFileName, -1);
    }

    gtk_widget_set_visible(GTK_WIDGET(symOutputFileChooserDialog), FALSE);
}

// Otwarcie dialogu zapisu
void symOutputButton_cb(GtkButton * symOutputButton, gpointer user_data)
{
    GtkWidget * symOutputFileChooserDialog = GTK_WIDGET(user_data);
    gtk_window_present(GTK_WINDOW(symOutputFileChooserDialog));
}

void symAlgorithmDropDown_cb(GtkDropDown * symAlgorithmDropDown, GParamSpec * pspec, gpointer user_data)
{
    int position = 0;

    GtkWidget * blockModeDropDown = NULL;
    GtkWidget * keySizeDropDown = NULL;

    blockModeDropDown = get_child_of_type(GTK_WIDGET(symAlgorithmDropDown), GTK_TYPE_DROP_DOWN, 1);
    keySizeDropDown = get_child_of_type(blockModeDropDown, GTK_TYPE_DROP_DOWN, 1);

    position = gtk_drop_down_get_selected(symAlgorithmDropDown);

    gtk_drop_down_set_model(GTK_DROP_DOWN(blockModeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_blockmodes()[position])));
    gtk_drop_down_set_selected(GTK_DROP_DOWN(blockModeDropDown), 0);
    gtk_drop_down_set_model(GTK_DROP_DOWN(keySizeDropDown), G_LIST_MODEL(gtk_string_list_new(symmetric_cipher_get_keysize()[position])));
    gtk_drop_down_set_selected(GTK_DROP_DOWN(keySizeDropDown), 0);
}
