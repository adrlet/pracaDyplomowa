#include <stdlib.h>
#include <openssl/bio.h>

#include "settings.h"
#include "settingsDialog.h"

#include "evp_salt.h"
#include "message_digest_map.h"
#include "widgetManagment.h"
#include "bio_pre.h"

#include "errorLog.h"
#include "jobsLog.h"
#include "log.h"

const char * envHomeDrive = "HOMEDRIVE";
const char * envHomePath = "HOMEPATH";

struct settings * applicationSettings = NULL;

const char * homeCatalog = "\\OpenSSLApp";

char * applicationPath = NULL;
int applicationPathLen = 0;

const char * logDirectory = "\\logs";
const char * errorLogName = "\\logs.txt";
const char * jobsLogName = "\\jobs.txt";

const char * keysDirectory = "\\keys";
const char * parametersDirectory = "\\parameters";

const char * settingsFileName = "\\conf.txt";

static void settings_set_home_path()
{
    char * homeDrive = getenv(envHomeDrive);
    char * homePath = getenv(envHomePath);
    char * buffer = NULL;

    if(applicationPath != NULL && applicationPathLen > 0)
        free(applicationPath);
    applicationPath = NULL;

    applicationPathLen = strlen(homeDrive)+strlen(homePath)+strlen(homeCatalog);
    buffer = (char*)malloc(sizeof(char)*(applicationPathLen+1));
    buffer[0] = 0;

    strcat(buffer, homeDrive);
    strcat(buffer, homePath);
    strcat(buffer, homeCatalog);

    applicationPath = buffer;
}

static struct settings * settings_new()
{
    struct settings * appSettings = NULL;

    appSettings = (struct settings*)malloc(sizeof(struct settings));
    if(appSettings == NULL)
        return appSettings;

    appSettings->saltLen = 0;
    appSettings->bufferSize = 0;
    appSettings->errorPath = NULL;
    appSettings->jobsPath = NULL;
    appSettings->keysPath = NULL;
    appSettings->parametersPath = NULL;
    appSettings->derive_algorithm = 0;

    return appSettings;
}

static void settings_clear(struct settings * appSettings)
{
    if(appSettings == NULL)
        return;

    appSettings->saltLen = 0;
    appSettings->bufferSize = 0;

    if(appSettings->errorPath != NULL)
        free(appSettings->errorPath);
    appSettings->jobsPath = NULL;
    if(appSettings->jobsPath != NULL)
        free(appSettings->jobsPath);
    appSettings->jobsPath = NULL;
    if(appSettings->keysPath != NULL)
        free(appSettings->keysPath);
    appSettings->keysPath = NULL;
    if(appSettings->parametersPath != NULL)
        free(appSettings->parametersPath);
    appSettings->parametersPath = NULL;

    appSettings->derive_algorithm = 0;
}

static void settings_set_default(struct settings * appSettings)
{
    char * buffer = NULL;
    int bufferLen = MAX_PATH_LEN;
    int homeLen = 0;
    int totalLen = 0;

    if(appSettings == NULL)
        return;

    appSettings->saltLen = DEFAULT_SALT_LEN;

    totalLen = applicationPathLen+strlen(logDirectory)+strlen(errorLogName);
    if(totalLen < bufferLen)
    {
        buffer = (char*)malloc(sizeof(char)*(bufferLen+1));
        buffer[0] = 0;
        strcat(buffer, applicationPath);
        strcat(buffer, logDirectory);
        strcat(buffer, errorLogName);
        appSettings->errorPath = buffer;
    }

    totalLen = applicationPathLen+strlen(logDirectory)+strlen(jobsLogName);
    if(totalLen < bufferLen)
    {
        buffer = (char*)malloc(sizeof(char)*(bufferLen+1));
        buffer[0] = 0;
        strcat(buffer, applicationPath);
        strcat(buffer, logDirectory);
        strcat(buffer, jobsLogName);
        appSettings->jobsPath = buffer;
    }

    totalLen = homeLen+strlen(keysDirectory);
    if(totalLen < bufferLen)
    {
        buffer = (char*)malloc(sizeof(char)*(bufferLen+1));
        buffer[0] = 0;
        strcat(buffer, applicationPath);
        strcat(buffer, keysDirectory);
        appSettings->keysPath = buffer;
    }

    totalLen = homeLen+strlen(parametersDirectory);
    if(totalLen < bufferLen)
    {
        buffer = (char*)malloc(sizeof(char)*(bufferLen+1));
        buffer[0] = 0;
        strcat(buffer, applicationPath);
        strcat(buffer, parametersDirectory);
        appSettings->parametersPath = buffer;
    }

    appSettings->bufferSize = DEFAULT_BUFFER_SIZE;

    appSettings->derive_algorithm = EVP_SHA384;
}

const char * settingsKeys[] =
{
    "saltLen=", // 0
    "bufferSize=", // 1
    "logsDirectory=", // 2
    "keysDirectory=", // 3
    "parametersDirectory=", // 4
    "deriveAlgorithm=" // 5
};

void settings_save_file(struct settings * appSettings)
{
    GFile * settingsFile = NULL;
    GFileOutputStream * settingsStream = NULL;

    char * buffer = NULL;
    char * terminator = NULL;
    int i = 0;
    int j =0;

    if(appSettings == NULL)
        goto end;

    buffer = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
    buffer[0] = 0;
    strcat(buffer, applicationPath);
    strcat(buffer, settingsFileName);

    settingsFile = g_file_new_for_path(buffer);
    settingsStream = g_file_replace(settingsFile, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
    if(settingsStream == NULL)
        goto end;
    free(buffer);
    buffer = NULL;

    buffer = (char*)malloc(sizeof(char)*(1024+1));
    sprintf(buffer, "%s%d%s", settingsKeys[0], appSettings->saltLen, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

    sprintf(buffer, "%s%d%s", settingsKeys[1], appSettings->bufferSize, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

    strcpy(buffer, settingsKeys[2]);
    j = strlen(buffer);
    terminator = strrchr(appSettings->errorPath, slash);
    for(i = 0; appSettings->errorPath+i != terminator; i++)
        buffer[i+j] = appSettings->errorPath[i];
    buffer[i+j] = 0;
    strcat(buffer, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

    sprintf(buffer, "%s%s%s", settingsKeys[3], appSettings->keysPath, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

    sprintf(buffer, "%s%s%s", settingsKeys[4], appSettings->parametersPath, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

    sprintf(buffer, "%s%d%s", settingsKeys[5], appSettings->derive_algorithm, newLine);
    g_output_stream_write(G_OUTPUT_STREAM(settingsStream), buffer, strlen(buffer), NULL, NULL);

end:
    if(buffer != NULL)
        free(buffer);
    if(settingsFile != NULL)
        g_object_unref(settingsFile);
    if(settingsStream != NULL)
        g_object_unref(settingsStream);
}

static int settings_read_line(GFileInputStream * settingsStream, char * buffer)
{
    int i = 0;

    do
    {
        g_input_stream_read(G_INPUT_STREAM(settingsStream), buffer+i, 1, NULL, NULL);
    }while(buffer[i++] != '\n');
    if(buffer[i-2] == '\r')
        i--;
    buffer[i-1] = 0;

    return i-1;
}

int settings_check_key(GFileInputStream * settingsStream, char * buffer, const char * settingsKey)
{
    int i = 0;

    i = g_input_stream_read(G_INPUT_STREAM(settingsStream), buffer, strlen(settingsKey), NULL, NULL);
    buffer[i] = 0;
    if(strcmp(buffer, settingsKey) != 0)
        return -1;
    return 0;
}

int settings_load_file(struct settings * appSettings)
{
    GFile * settingsFile = NULL;
    GFileInputStream * settingsStream = NULL;

    char * buffer = NULL;
    int iResult = 0;
    int i = 0;

    if(appSettings == NULL)
        goto error;

    buffer = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
    buffer[0] = 0;
    strcat(buffer, applicationPath);
    strcat(buffer, settingsFileName);
    settingsFile = g_file_new_for_path(buffer);
    settingsStream = g_file_read(settingsFile, NULL, NULL);
    if(settingsStream == NULL)
        goto error;
    free(buffer);
    buffer = NULL;

    buffer = (char*)malloc(sizeof(char)*(1024+1));
    iResult = settings_check_key(settingsStream, buffer, settingsKeys[0]);
    if(iResult == -1)
        goto error;
    settings_read_line(settingsStream, buffer);
    i = string_to_int(buffer);
    if(i > 0)
        appSettings->saltLen = i;

    iResult = settings_check_key(settingsStream, buffer, settingsKeys[1]);
    if(iResult == -1)
        goto error;
    settings_read_line(settingsStream, buffer);
    i = string_to_int(buffer);
    if(i >= DEFAULT_BUFFER_SIZE)
        appSettings->bufferSize = i;

    iResult = settings_check_key(settingsStream, buffer, settingsKeys[2]);
    if(iResult == -1)
        goto error;
    i = settings_read_line(settingsStream, buffer);
    if(g_file_test(buffer, G_FILE_TEST_EXISTS) == FALSE)
        goto error;
    strcat(buffer, errorLogName);
    appSettings->errorPath = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(appSettings->errorPath, buffer);
    buffer[i] = 0;
    strcat(buffer, jobsLogName);
    appSettings->jobsPath = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(appSettings->jobsPath, buffer);

    iResult = settings_check_key(settingsStream, buffer, settingsKeys[3]);
    if(iResult == -1)
        goto error;
    i = settings_read_line(settingsStream, buffer);
    if(g_file_test(buffer, G_FILE_TEST_EXISTS) == FALSE)
        goto error;
    appSettings->keysPath = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(appSettings->keysPath, buffer);

    iResult = settings_check_key(settingsStream, buffer, settingsKeys[4]);
    if(iResult == -1)
        goto error;
    i = settings_read_line(settingsStream, buffer);
    if(g_file_test(buffer, G_FILE_TEST_EXISTS) == FALSE)
        goto error;
    appSettings->parametersPath = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(appSettings->parametersPath, buffer);

    iResult = settings_check_key(settingsStream, buffer, settingsKeys[5]);
    if(iResult == -1)
        goto error;
    settings_read_line(settingsStream, buffer);
    i = string_to_int(buffer);
    if(i >= 0 && i < MESSAGE_DIGEST_ALGORITHMS)
        appSettings->derive_algorithm = i;

    iResult = 0;
    goto end;

error:
    iResult = -1;
end:
    if(buffer != NULL)
        free(buffer);
    if(settingsFile != NULL)
        g_object_unref(settingsFile);
    if(settingsStream != NULL)
        g_object_unref(settingsStream);

    return iResult;
}

int settings_init(GtkWidget * mainWindow)
{
    int iResult = 0;
    GFile * directory = NULL;
    char * buffer = NULL;

    if(mainWindow == NULL)
        goto error;

    settings_set_home_path();

    applicationSettings = settings_new();
    printf("settings_init settings_new ok\n");
    settings_dialog_init(mainWindow);
    printf("settings_init settings_dialog_init ok\n");
    iResult = settings_load_file(applicationSettings);
    if(iResult == -1)
    {
        printf("failed loading settings\n");
        settings_clear(applicationSettings);
        printf("settings_init settings_clear ok\n");
        settings_set_default(applicationSettings);
        printf("settings_init settings_set_default ok\n");

        buffer = (char*)malloc(sizeof(char)*(MAX_PATH_LEN+1));
        buffer[0] = 0;
        strcat(buffer, applicationPath);;
        printf("directory %s\n", buffer);
        directory = g_file_new_for_path(buffer);
        g_file_make_directory(directory, NULL, NULL);
        g_object_unref(directory);
        directory = NULL;

        strcat(buffer, logDirectory);
        printf("logDir %s\n", buffer);
        directory = g_file_new_for_path(buffer);
        g_file_make_directory(directory, NULL, NULL);
        g_object_unref(directory);
        directory = NULL;
        free(buffer);
        buffer = NULL;

        directory = g_file_new_for_path(applicationSettings->keysPath);
        g_file_make_directory(directory, NULL, NULL);
        g_object_unref(directory);
        directory = NULL;

        directory = g_file_new_for_path(applicationSettings->parametersPath);
        g_file_make_directory(directory, NULL, NULL);
        g_object_unref(directory);
        directory = NULL;

        settings_save_file(applicationSettings);
        printf("settings_init directory ok\n");
    }
    else
        printf("settings_init settings_loadFile ok\n");

    error_log_set_path(applicationSettings->errorPath);
    jobs_log_set_path(applicationSettings->jobsPath);
    setBufferSize(applicationSettings->bufferSize);

    iResult = 0;
    goto end;

error:
    iResult = -1;
end:
    if(buffer != NULL)
        free(buffer);
    if(directory != NULL)
        g_object_unref(directory);

    return iResult;
}

struct settings * settings_get_settings()
{
    return applicationSettings;
};

const char * settings_get_error_log_name()
{
    return errorLogName;
}

const char * settings_get_jobs_log_name()
{
    return jobsLogName;
}

unsigned char settings_get_derive_algorithm()
{
    return applicationSettings->derive_algorithm;
}

void settings_provide_data(struct settings * appSettings, struct settingsPanel * panel)
{
    GtkEntryBuffer * entryBuffer = NULL;
    char * buffer = NULL;
    int i = 0;
    char * terminator = NULL;

    if(appSettings == NULL || panel == NULL)
        return;

    buffer = (char*)malloc(sizeof(char)*(1024+1));

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->saltLenEntry));
    sprintf(buffer, "%d", appSettings->saltLen);
    gtk_entry_buffer_set_text(entryBuffer, buffer, -1);

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->bufferSizeEntry));
    sprintf(buffer, "%d", appSettings->bufferSize);
    gtk_entry_buffer_set_text(entryBuffer, buffer, -1);

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->logsPathEntry));
    terminator = strrchr(appSettings->errorPath, slash);
    for(i = 0; appSettings->errorPath+i != terminator; i++)
        buffer[i] = appSettings->errorPath[i];
    buffer[i] = 0;
    gtk_entry_buffer_set_text(entryBuffer, buffer, -1);

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->keysDirectoryEntry));
    gtk_entry_buffer_set_text(entryBuffer, appSettings->keysPath, -1);

    entryBuffer = gtk_entry_get_buffer(GTK_ENTRY(panel->parametersDirectoryEntry));
    gtk_entry_buffer_set_text(entryBuffer, appSettings->parametersPath, -1);

    gtk_drop_down_set_selected(GTK_DROP_DOWN(panel->derive_algorithmDropDown), appSettings->derive_algorithm);
}
