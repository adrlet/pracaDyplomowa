#include <string.h>
#include <pthread.h>

#include "symmetricDeriveKey.h"
#include "symmetricDeriveKey_struct.h"

#include "evp_derivekey.h"
#include "evp_derivekey_struct.h"
#include "evp_async_derivekey.h"
#include "evp_async_derivekey_struct.h"

#include "widgetManagment.h"
#include "passwordFilter.h"
#include "message_digest_map.h"
#include "widgetAsync.h"
#include "jobsLog.h"

const char * deriveKeyOperation = "Derive Key";

gboolean symmetric_derive_key_spiner_close_cb(GtkDialog * spinnerDialog, gpointer user_data)
{
    return TRUE;
}

gboolean symmetric_derive_key_password_close_cb(GtkDialog * passwordDialog, gpointer user_data)
{
    struct symmetric_derive_key * derive_key_data = (struct symmetric_derive_key*)user_data;

    derive_key_data->user_cleaner(derive_key_data->user_data);
    symmetric_derive_key_clear(derive_key_data);

    printf("symmetric_derive_key_password_close_cb\n");

    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);
    return TRUE;
}

static void symmetric_derive_key_error(struct symmetric_derive_key * derive_key_data, int errorCode)
{
    printf("symmetric_derive_key_error\n");

    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(derive_key_data->alertDialog), "Derywacja klucza");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(derive_key_data->alertDialog),
                                             "Niepowodzenie, kod błędu: %d", errorCode);

    gtk_window_present(GTK_WINDOW(derive_key_data->alertDialog));
}

static void _symmetric_derive_key_finish(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data)
{
    int iResult = g_task_propagate_int(G_TASK(res), NULL);

    struct symmetric_derive_key * derive_key_data = (struct symmetric_derive_key*)user_data;
    void * next_data = derive_key_data->user_data;
    void (*user_function)(void*) = derive_key_data->user_function;

    printf("_symmetric_derive_key_finish %d\n", iResult);
    gtk_widget_set_visible(GTK_WIDGET(spinnerDialog), FALSE);

    if(iResult < 1)
    {
        derive_key_data->user_cleaner(derive_key_data->user_data);
        symmetric_derive_key_clear(derive_key_data);
        symmetric_derive_key_error(derive_key_data, -iResult);
    }
    else
    {
        derive_key_data->transfer_data(derive_key_data->user_data, derive_key_data->key, derive_key_data->keylen, derive_key_data->iv, derive_key_data->ivlen);
        symmetric_derive_key_clear(derive_key_data);
        user_function(next_data);
    }
}

static int _symmetric_derive_key_func(GObject * spinnerDialog, GAsyncResult * res, gpointer user_data, GCancellable * cancellable)
{
    struct symmetric_derive_key * derive_key_data = (struct symmetric_derive_key*)user_data;

    printf("_symmetric_derive_key_func\n");

    int * iResult = NULL;
    int realResult = 0;

    GtkWidget * spinnerDialog2 = GTK_WIDGET(spinnerDialog);
    GtkWidget * spinnerDialogContent = gtk_dialog_get_content_area(GTK_DIALOG(spinnerDialog2));
    GtkWidget * spinnerLabel = NULL;
    GtkWidget * spinner = NULL;

    struct spinner_async_provider * spinner_provider = NULL;

    pthread_t thread;
    struct async_deriveKey_data * deriveKey_data = NULL;

    spinnerLabel = get_child_of_type(spinnerDialogContent, GTK_TYPE_LABEL, 0);
    if(spinnerLabel == NULL)
        goto error;

    spinner = get_child_of_type(spinnerLabel, GTK_TYPE_SPINNER, 1);
    if(spinner == NULL)
        goto error;

    deriveKey_data = async_deriveKey_data_new();
    if(deriveKey_data == NULL)
        goto error;

    async_deriveKey_data_provide(deriveKey_data, derive_key_data->password, derive_key_data->saltpath, EVP_SHA384,
                                 derive_key_data->keylen, derive_key_data->ivlen, derive_key_data->saltlen, derive_key_data->flags);
    async_deriveKey_data_init_buffers(deriveKey_data);

    spinner_async_provider_new(spinner_provider, spinner, TRUE);

    spinner_provider->instances++;
    g_idle_add(spinner_async_update, spinner_provider);

    pthread_create(&thread, NULL, async_derive_key, deriveKey_data);
    pthread_join(thread, (void**)&iResult);

    spinner_provider->spin = FALSE;
    spinner_provider->instances++;
    g_idle_add(spinner_async_update, spinner_provider);

    symmetric_derive_key_provide_data(derive_key_data, deriveKey_data->key, deriveKey_data->keylen, deriveKey_data->iv, deriveKey_data->ivlen, 2048);

error:

//end:
    if(iResult != NULL)
    {
        realResult = iResult[0];
        free(iResult);
    }
    else
        realResult = 0;

    if(realResult == 1)
        jobs_log_async_write(deriveKeyOperation, 0);
    else
        jobs_log_async_write(deriveKeyOperation, 1);

    if(spinner_provider != NULL)
    {
        if(spinner_provider->instances == 0)
        {
            free(spinner_provider);
            printf("spinner idle freed\n");
        }
        else
            spinner_provider->spinner = NULL;
    }

    if(deriveKey_data != NULL)
        async_deriveKey_data_delete(deriveKey_data);

    return realResult;
}

static void _symmetric_derive_key_thread(GTask * task, void * spinnerDialog, gpointer user_data, GCancellable * cancellable)
{
    int iResult = 1;

    printf("_symmetric_derive_key_thread\n");

    iResult = _symmetric_derive_key_func(spinnerDialog, G_ASYNC_RESULT(task), user_data, cancellable);

    g_task_return_int(task, iResult);
}

static void _symmetric_derive_key_async(GtkWidget * spinnerDialog, GCancellable * cancellable, GAsyncReadyCallback arc, gpointer user_data)
{
    GTask * task = g_task_new(spinnerDialog, cancellable, arc, user_data);

    printf("_symmetric_derive_key_async\n");

    g_task_set_task_data(task, user_data, (GDestroyNotify)NULL);
    g_task_set_return_on_cancel(task, TRUE);
    g_task_run_in_thread(task, _symmetric_derive_key_thread);

    g_object_unref(task);
}

static void symmetric_derive_key_init(gpointer user_data)
{
    struct symmetric_derive_key * derive_key_data = (struct symmetric_derive_key*)user_data;

    gtk_window_present(GTK_WINDOW(derive_key_data->spinnerDialog));

    _symmetric_derive_key_async(derive_key_data->spinnerDialog, NULL, _symmetric_derive_key_finish, user_data);
}

void symmetric_derive_key_cb(GtkDialog * passwordDialog, gint response_id, gpointer user_data)
{
    if(response_id != GTK_RESPONSE_OK)
        return;

    struct symmetric_derive_key * derive_key_data = (struct symmetric_derive_key*)user_data;

    printf("symmetric_derive_key_cb\n");

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

    symmetric_derive_key_provide_password(derive_key_data, password);

    gtk_label_set_text(GTK_LABEL(passwordFilterLabel), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordEntry), "");
    gtk_editable_set_text(GTK_EDITABLE(passwordRepeatEntry), "");
    gtk_widget_set_visible(GTK_WIDGET(passwordDialog), FALSE);

    symmetric_derive_key_init(user_data);
}
