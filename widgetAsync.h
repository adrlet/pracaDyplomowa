#ifndef WIDGETASYNC_H_INCLUDED
#define WIDGETASYNC_H_INCLUDED

#include <gtk/gtk.h>

#define progress_bar_async_provider_new(provider, widgetProgressBar, valueProgress) do { \
    provider = (struct progress_bar_async_provider*)malloc(sizeof(struct progress_bar_async_provider)); \
    if(provider == NULL) \
        goto error; \
    provider->progressBar = GTK_PROGRESS_BAR(widgetProgressBar); \
    provider->progress = valueProgress; \
    provider->instances = 0; \
} while(0)

#define label_async_provider_new(provider, widgetLabel, valueText) do { \
    provider = (struct label_async_provider*)malloc(sizeof(struct label_async_provider)); \
    if(provider == NULL) \
        goto error; \
    provider->label = GTK_LABEL(widgetLabel); \
    provider->text = valueText; \
    provider->instances = 0; \
} while(0)

#define spinner_async_provider_new(provider, widgetSpinner, valueSpin) do { \
    provider = (struct spinner_async_provider*)malloc(sizeof(struct spinner_async_provider)); \
    if(provider == NULL) \
        goto error; \
    provider->spinner = GTK_SPINNER(widgetSpinner); \
    provider->spin = valueSpin; \
    provider->instances = 0; \
} while(0)

#define async_provider_free(provider) do { \
    if(provider->instances == 0) \
        free(provider); \
} while(0)

struct progress_bar_async_provider
{
    GtkProgressBar * progressBar; // 8
    double * progress; // 16
    int instances; // 20
};

struct label_async_provider
{
    GtkLabel * label; // 8
    char * text; // 16
    int instances; // 20
};

struct spinner_async_provider
{
    GtkSpinner * spinner; // 8
    gboolean spin; // 12
    int instances; // 16
};

// Asynchroniczna aktualizacja progress baru
gboolean progress_bar_async_update(gpointer user_data);

// Asynchroniczna aktualizacja labela
gboolean label_async_update(gpointer user_data);

gboolean spinner_async_update(gpointer user_data);

#endif // WIDGETASYNC_H_INCLUDED
