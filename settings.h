#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <gtk/gtk.h>

#define MAX_PATH_LEN 256
#define DEFAULT_BUFFER_SIZE 4096

struct settings
{
    int saltLen; // 4
    int bufferSize; // 8
    char * errorPath; // 16
    char * jobsPath; // 24
    char * keysPath; // 32
    char * parametersPath; // 40
    unsigned char derive_algorithm; // 41
};

struct settingsPanel;

void settings_save_file(struct settings * appSettings);

int settings_load_file(struct settings * appSettings);

int settings_init(GtkWidget * mainWindow);

struct settings * settings_get_settings();

const char * settings_get_error_log_name();

const char * settings_get_jobs_log_name();

unsigned char settings_get_derive_algorithm();

void settings_provide_data(struct settings * appSettings, struct settingsPanel * panel);

#endif // SETTINGS_H_INCLUDED
