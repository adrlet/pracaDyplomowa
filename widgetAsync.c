#include "widgetAsync.h"

gboolean progress_bar_async_update(gpointer user_data)
{
    if(user_data == NULL)
        return G_SOURCE_REMOVE;

    struct progress_bar_async_provider * provider = (struct progress_bar_async_provider*)user_data;

    printf("progress instances: %d\n", provider->instances);
    provider->instances--;
    if(provider->progressBar == NULL)
    {
        async_provider_free(provider);
        return G_SOURCE_REMOVE;
    }

    if(provider->progress == NULL)
        return G_SOURCE_REMOVE;

    if(GTK_IS_PROGRESS_BAR(provider->progressBar))
        gtk_progress_bar_set_fraction(provider->progressBar, *(provider->progress));

    return G_SOURCE_REMOVE;
}

gboolean label_async_update(gpointer user_data)
{
    if(user_data == NULL)
        return G_SOURCE_REMOVE;

    struct label_async_provider * provider = (struct label_async_provider*)user_data;

    printf("label instances: %d\n", provider->instances);
    provider->instances--;
    if(provider->label == NULL)
    {
        async_provider_free(provider);
        return G_SOURCE_REMOVE;
    }

    if(provider->text == NULL)
        return G_SOURCE_REMOVE;

    if(GTK_IS_LABEL(provider->label))
        gtk_label_set_text(provider->label, provider->text);

    return G_SOURCE_REMOVE;
}

gboolean spinner_async_update(gpointer user_data)
{
    if(user_data == NULL)
        return G_SOURCE_REMOVE;

    struct spinner_async_provider * provider = (struct spinner_async_provider*)user_data;

    printf("spinner instances: %d\n", provider->instances);
    provider->instances--;
    if(provider->spinner == NULL)
    {
        async_provider_free(provider);
        return G_SOURCE_REMOVE;
    }

    if(GTK_IS_SPINNER(provider->spinner))
    {
        if(provider->spin == TRUE)
            gtk_spinner_start(provider->spinner);
        else
            gtk_spinner_stop(provider->spinner);
    }

    return G_SOURCE_REMOVE;
}
