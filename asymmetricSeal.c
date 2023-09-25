#include <pthread.h>

#include "asymmetricSeal.h"
#include "asymmetricSeal_struct.h"
#include "asymmetricSealPanel_struct.h"
#include "pkey_async_seal.h"
#include "pkey_async_seal_struct.h"

#include "thread_asynchronichandler.h"
#include "message_digest_map.h"
#include "asymmetric_algorithm_map.h"
#include "symmetric_cipher_map.h"
#include "widgetManagment.h"
#include "widgetAsync.h"
#include "jobsLog.h"
#include "log.h"
#include "passwordFilter.h"

const char * assymetricSealOperation = "asymmetric_seal";

void _asymmetric_seal_progress_cancel_cb(GtkButton * progressButton, gpointer user_data)
{
    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(data->progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    printf("_asymmetric_seal_progress_cancel_cb\n");

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    g_cancellable_cancel(data->cancel);
    gtk_widget_set_visible(GTK_WIDGET(data->progressBox), FALSE);
}

static void _asymmetric_seal_ok(struct asymmetric_seal_data * data, const char * filename, const char * operationString)
{
    printf("_asymmetric_seal_ok\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                             "Operacja zakończona pomyślnie %s", filename);
}

static void _asymmetric_seal_error(struct asymmetric_seal_data * data, const char * filename, int errorCode, const char * operationString)
{
    printf("_asymmetric_seal_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(data->alertDialog), operationString);
    if(errorCode == 1)
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Przerwano");
    else
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(data->alertDialog),
                                                 "Niepowodzenie, kod błędu: %d", errorCode);
}

static void _asymmetric_seal_progress_finish(GObject * progressBox, GAsyncResult * res, gpointer user_data)
{
    const char * sealSeal = "Tworzenie koperty";
    const char * sealOpen = "Otwieranie";
    const char * operationString = NULL;
    int mode = 0;
    char * inputFileName = NULL;

    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    gtk_label_set_text(GTK_LABEL(progressLabel), logs_get_progressText());
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), 0.0);

    gtk_widget_set_visible(GTK_WIDGET(progressBox), FALSE);

    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    mode = (data->flags & ASSYMETRIC_SEAL_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_SEAL_SEAL:
        operationString = sealSeal;
        break;
    case ASSYMETRIC_SEAL_OPEN:
        operationString = sealOpen;
        break;
    }

    inputFileName = getFileName(data->inputPath);

    if(iResult == 1)
        _asymmetric_seal_ok(data, inputFileName, operationString);
    else
        _asymmetric_seal_error(data, inputFileName, -iResult, operationString);

    if(inputFileName != NULL)
        free(inputFileName);

    gtk_window_present(GTK_WINDOW(data->alertDialog));
}

static char ** asymmetric_seal_get_keys(gpointer user_data)
{
    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    GFile * pubKeysDir = NULL;
    GFileEnumerator * pubKeys = NULL;
    GFileInfo * pubKey = NULL;
    const char * pubKeyName = NULL;
    char ** pubKeyNames = NULL;
    int i = 0;

    pubKeysDir = g_file_new_for_path(data->pubKeyPath);
    pubKeys = g_file_enumerate_children(pubKeysDir, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if(pubKeys == NULL)
        goto end;

    i = 0;
    while(g_file_enumerator_next_file(pubKeys, NULL, NULL) != NULL)
        i++;

    data->keynum = i;
    if(i == 0)
        goto end;
    g_object_unref(pubKeys);
    pubKeyNames = (char **)malloc(sizeof(char*)*i);
    memset(pubKeyNames, 0, sizeof(char*)*i);
    pubKeys = g_file_enumerate_children(pubKeysDir, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);

    i = 0;
    pubKey = g_file_enumerator_next_file(pubKeys, NULL, NULL);
    while(pubKey != NULL)
    {
        pubKeyName = g_file_info_get_name(pubKey);
        if(pubKeyName != NULL)
        {
            pubKeyNames[i] = (char*)malloc(sizeof(char)*(strlen(pubKeyName)+strlen(data->pubKeyPath)+1));
            pubKeyNames[i][0] = 0;
            strcat(pubKeyNames[i], data->pubKeyPath);
            strcat(pubKeyNames[i], "\\");
            strcat(pubKeyNames[i], pubKeyName);
        }
        i++;
        pubKey = g_file_enumerator_next_file(pubKeys, NULL, NULL);
    }

end:
    if(pubKeys != NULL)
        g_object_unref(pubKeys);
    if(pubKeysDir != NULL)
        g_object_unref(pubKeysDir);

    return pubKeyNames;
}

static int _asymmetric_seal_progress_func(GObject * progressBox, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    GtkWidget * progressLabel = get_child_of_type(get_child_of_type(GTK_WIDGET(progressBox), GTK_TYPE_LABEL, 0), GTK_TYPE_LABEL, 1);
    GtkWidget * progressBar = get_child_of_type(progressLabel, GTK_TYPE_PROGRESS_BAR, 1);

    int * iResult = NULL;
    int realResult = 0;
    pthread_t thread;
    int mode = 0;
    int i =0;

    struct progress_bar_async_provider * progress_bar_provider = NULL;
    struct label_async_provider * label_provider = NULL;

    clock_t interval;
    double progress = 0.0;
    long double fileSize = 0.0;
    char * progressPercent = NULL;
    const int progressPercentLen = 15;

    struct asynchronicHandler * handler = NULL;
    struct async_seal_data * async_data = NULL;
    const char ** pubKeys = NULL;

    printf("_asymmetric_seal_progress_func\n");

    fileSize = (long double)getFileSize(data->inputPath);
    progressPercent = (char*)malloc(sizeof(char)*(progressPercentLen+1));
    strcpy(progressPercent, logs_get_progressText());

    progress_bar_async_provider_new(progress_bar_provider, progressBar, &progress);
    label_async_provider_new(label_provider, progressLabel, progressPercent);

    handler = setupAsynchronicHandler();
    if(handler == NULL)
        goto end;

    async_data = async_seal_data_new();
    if(async_data == NULL)
        goto error;

    mode = (data->flags & ASSYMETRIC_SEAL_MODE_MASK);
    if(mode == ASYMMETRIC_SEAL_SEAL)
        pubKeys = asymmetric_seal_get_keys(data);
    else
        data->keynum = 1;
    async_seal_data_provide(async_data, data->inputPath, data->outputPath, pubKeys, data->privKeyPath, data->ivPath, data->keyPath,
                            data->passphrase, handler, data->keynum, CIPHER_FULL_ADDRESS(data->algorithm, data->keySize, data->blockMode),
                            CIPHER_FULL_ADDRESS(data->privAlgorithm, data->privKeySize, data->privBlockMode), data->flags);

    switch(mode)
    {
    case ASYMMETRIC_SEAL_SEAL:
        pthread_create(&thread, NULL, async_seal, async_data);
        break;
    case ASSYMETRIC_SEAL_OPEN:
        pthread_create(&thread, NULL, async_open, async_data);
        break;
    }

    interval = clock();
    while(progress < 1.0)
    {
        while(clock()-interval < CLOCKS_PER_SEC)
            continue;
        interval = clock();

        progress = (double)(((long double)handler->processed)/fileSize);
        sprintf(progressPercent+logs_get_progressTextLen(), "%0.f%%", round(progress*100.f));

        if(g_cancellable_is_cancelled(cancellable))
            handler->interrupt = 1;
        if(handler->interrupt == 1)
            goto interrupted;

        progress_bar_provider->instances++;
        g_idle_add(progress_bar_async_update, progress_bar_provider);

        label_provider->instances++;
        g_idle_add(label_async_update, label_provider);

        printf("Processed: %llu\n", handler->processed);
    }

interrupted:
    pthread_join(thread, (void**)&iResult);

error:

end:
    if(iResult != NULL)
    {
        realResult = iResult[0];
        free(iResult);
    }
    else
        realResult = 0;

    switch(realResult)
    {
    case 1:
        jobs_log_async_write(assymetricSealOperation, 0);
        break;
    case -1:
        jobs_log_async_write(assymetricSealOperation, 2);
        break;
    default:
        jobs_log_async_write(assymetricSealOperation, 1);
    }

    if(progress_bar_provider != NULL)
    {
        if(progress_bar_provider->instances == 0)
        {
            free(progress_bar_provider);
            printf("progress bar idle freed\n");
        }
        else
            progress_bar_provider->progressBar = NULL;
    }
    if(label_provider != NULL)
    {
        if(label_provider->instances == 0)
        {
            free(label_provider);
            printf("label idle freed\n");
        }
        else
            label_provider->label = NULL;
    }

    if(progressPercent != NULL)
        free(progressPercent);
    if(handler != NULL)
        freeAsynchronicHandler(handler);
    if(async_data != NULL)
        async_seal_data_delete(async_data);
    if(pubKeys != NULL)
    {
        for(i = 0; i < data->keynum; i++)
        {
            if(pubKeys[i] == NULL)
                continue;
            memset(pubKeys[i], 0, sizeof(char)*strlen(pubKeys[i]));
            free(pubKeys[i]);
            pubKeys[i] = NULL;
        }
        free(pubKeys);
    }

    return realResult;
}

static void _asymmetric_seal_progress_thread(GTask * task, void * progressBox, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_asymmetric_seal_progress_thread\n");

    iResult = _asymmetric_seal_progress_func(progressBox, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _asymmetric_seal_progress_async(GtkWidget * progressBox, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(progressBox, cancellable, arc, user_data);

    printf("_asymmetric_seal_progress_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)asymmetric_seal_data_clear);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _asymmetric_seal_progress_thread);

    g_object_unref(task);
}

static void asymmetric_seal_progress_init(gpointer user_data)
{
    const char * operationString = NULL;
    static const char * sealSeal = "Tworzenie kopert...";
    static const char * sealCheck = "Otwieranie...";
    int mode = 0;

    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;
    GtkWidget * progressBox = data->progressBox;
    GtkWidget * progressName = get_child_of_type(data->progressBox, GTK_TYPE_LABEL, 0);

    printf("asymmetric_seal_progress_init\n");

    mode = (data->flags & ASSYMETRIC_SEAL_MODE_MASK);
    switch(mode)
    {
    case ASYMMETRIC_SEAL_SEAL:
        operationString = sealSeal;
        break;
    case ASSYMETRIC_SEAL_OPEN:
        operationString = sealCheck;
        break;
    }

    gtk_label_set_text(GTK_LABEL(progressName), operationString);

    gtk_widget_set_visible(progressBox, TRUE);

    _asymmetric_seal_progress_async(progressBox, data->cancel, _asymmetric_seal_progress_finish, user_data);
}

void asymmetric_seal_password_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data)
{
    if(response_id != GTK_RESPONSE_OK)
        return;

    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    printf("asymmetric_seal_password_cb\n");

    GtkWidget * passwordDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(passwordDialog));
    GtkWidget * passwordFilterLabel = NULL;
    GtkWidget * passwordEntry = NULL;
    GtkWidget * passwordRepeatEntry = NULL;

    const char * password = NULL;
    const char * passwordRepeat = NULL;
    int passwordResult = 0;

    passwordFilterLabel = get_child_of_type(passwordDialogContent, GTK_TYPE_LABEL, 0);
    if(passwordFilterLabel == NULL)
        return;

    passwordEntry = get_child_of_type(passwordFilterLabel, GTK_TYPE_PASSWORD_ENTRY, 1);
    if(passwordEntry == NULL)
        return;

    passwordRepeatEntry = get_child_of_type(passwordEntry, GTK_TYPE_PASSWORD_ENTRY, 1);
    if(passwordRepeatEntry == NULL)
        return;

    password = gtk_editable_get_text(GTK_EDITABLE(passwordEntry));
    passwordRepeat = gtk_editable_get_text(GTK_EDITABLE(passwordRepeatEntry));

    passwordResult = check_password(password, passwordRepeat);
    switch(passwordResult)
    {
    case PASS_TOO_SHORT:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło ma mniej niż 8 znaków");
        break;
    case PASS_TOO_LONG:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło ma więcej niż 256 znaków");
        break;
    case PASS_DOESNT_CONTAIN_SPECIAL_CHAR:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada znaku specjalnego");
        break;
    case PASS_DOESNT_CONTAIN_LOW_LETTER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada małej litery");
        break;
    case PASS_DOESNT_CONTAIN_UP_LETTER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada wielkiej litery");
        break;
     case PASS_DOESNT_CONTAIN_NUMBER:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie posiada cyfry");
        break;
    case PASS_IS_EMPTY:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło jest puste");
        break;
    case PASS_RETRY_IS_EMPTY:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Powtórz hasło");
        break;
    case PASS_AND_PASS_RETRY_ARE_DIFFERENT:
        gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "Hasło nie zgadzają się");
        break;
    default:
    }

    if(passwordResult != PASS_IS_OK)
        return;

    asymmetric_seal_data_provide_password(data, password);

    gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordEntry), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordRepeatEntry), "");
    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);

    asymmetric_seal_progress_init(user_data);
}

gboolean asymmetric_seal_password_close_cb(GtkDialog * passwordDialog, gpointer user_data)
{
    struct asymmetric_seal_data * data = (struct asymmetric_seal_data*)user_data;

    asymmetric_seal_data_clear(data);

    printf("asymmetric_seal_password_close_cb\n");

    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);
    return TRUE;
}

static void asymmetric_seal_proceed(gpointer user_data, unsigned char mode)
{
    struct asymmetric_seal_wrapper * wrapper = (struct asymmetric_seal_wrapper*)user_data;
    struct asymmetric_seal_data * data = wrapper->data;

    int iResult = 0;

    if(data->flags & IN_USE)
        return;

    printf("asymmetric_seal_proceed\n");

    asymmetric_seal_data_provide_flags(data, mode | IN_USE);
    asymmetric_seal_wrapper_provide_data(wrapper, data);
    iResult = asymmetric_seal_data_verify(data);
    if(iResult != ASSYMETRIC_SEAL_OK)
    {
        asymmetric_seal_data_clear(data);
        return;
    }

    if(mode == ASSYMETRIC_SEAL_OPEN)
        gtk_window_present(GTK_WINDOW(data->passwordDialog));
    else
        asymmetric_seal_progress_init(data);
}

void asymmetric_seal_seal(GtkButton * sealButton, gpointer user_data)
{
    printf("asymmetric_seal_seal\n");
    asymmetric_seal_proceed(user_data, ASYMMETRIC_SEAL_SEAL);
}

void asymmetric_seal_open(GtkButton * verifyButton, gpointer user_data)
{
    printf("asymmetric_seal_open\n");
    asymmetric_seal_proceed(user_data, ASSYMETRIC_SEAL_OPEN);
}
