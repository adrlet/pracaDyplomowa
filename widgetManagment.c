#include "widgetManagment.h"

void empty_box(GtkWidget * box)
{
    GtkWidget * children = NULL;

    if(box == NULL)
        return;

    children = gtk_widget_get_first_child(box);
    while(children != NULL)
    {
        gtk_box_remove(GTK_BOX(box), children);
        children = gtk_widget_get_first_child(box);
    }
}

GtkWidget * get_child_of_type(GtkWidget * box, GType type, unsigned char sibling)
{
    GtkWidget * child = NULL;

    if(box == NULL)
        return child;

    child = (sibling == 0 ? gtk_widget_get_first_child(box) : gtk_widget_get_next_sibling(box));
    while(child != NULL && G_OBJECT_TYPE(child) != type)
        child = gtk_widget_get_next_sibling(child);

    return child;
}

GtkWidget * get_parent_of_type(GtkWidget * child, GType type)
{
    GtkWidget * parent = NULL;

    if(child == NULL)
        return parent;

    parent = gtk_widget_get_parent(child);
    while(parent != NULL && G_OBJECT_TYPE(parent) != type)
        parent = gtk_widget_get_parent(parent);

    return parent;
}

void humanReadableBytes(unsigned long long fileSize, char * inputEntry_cbBuffer)
{
    if(inputEntry_cbBuffer == NULL)
        return;

    static const char * sufixes[] = { "B", "kB", "MB", "GB", "TB" };
    double value = 0.0;
    double remainder = 0.0;
    int mode = 0;

    value = (double)fileSize;
    while(value > 1000.0 && mode < 4)
    {
        value = value/1000.0;
        remainder = value - floor(value);
        mode++;
    }

    if(mode > 0)
    {
        remainder = round(remainder*100.0)/100.0;
        value = floor(value)+remainder;
        sprintf(inputEntry_cbBuffer, "Size: %.2f %s", value, sufixes[mode]);
    }
    else
        sprintf(inputEntry_cbBuffer, "Size: %llu %s", fileSize, sufixes[mode]);
}

long long unsigned int getFileSize(const char * filePath)
{
    if(filePath == NULL)
        return 0 ;

    long long unsigned int fileSize = 0;
    GFile * inputFile = NULL;
    GFileInfo * inputFileInfo = NULL;

    inputFile = g_file_new_for_path(filePath);
    inputFileInfo = g_file_query_info(inputFile, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if(inputFileInfo == NULL)
        goto end;
    fileSize = g_file_info_get_size(inputFileInfo);

end:
    if(inputFile != NULL)
        g_object_unref(inputFile);
    if(inputFileInfo != NULL)
        g_object_unref(inputFileInfo);

    return fileSize;
}

char * getFileName(const char * filePath)
{
    if(filePath == NULL)
        return NULL;

    char * returnedName = NULL;
    const char * inputFileName = NULL;
    GFile * inputFile = NULL;
    GFileInfo * inputFileInfo = NULL;

    inputFile = g_file_new_for_path(filePath);
    inputFileInfo = g_file_query_info(inputFile, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if(inputFileInfo == NULL)
        goto end;
    inputFileName = g_file_info_get_display_name(inputFileInfo);
    if(inputFileName == NULL)
        goto end;

    returnedName = (char*)malloc(sizeof(char)*(strlen(inputFileName)+1));
    if(returnedName == NULL)
        goto end;
    strcpy(returnedName, inputFileName);

end:
    if(inputFile != NULL)
        g_object_unref(inputFile);
    if(inputFileInfo != NULL)
        g_object_unref(inputFileInfo);

    return returnedName;
}

int skipSeparator(const char * record, int skipped, const char * separator)
{
    int i = 0;
    int count = 0;
    const char * iterator = NULL;

    if(skipped < 1)
        return count;

    iterator = record;
    while(i++ < skipped)
    {
        count = count+strcspn(iterator, separator)+1;
        iterator = record+count;
    }

    return count;
}

int string_to_int(const char * numeric)
{
    int i = 0;
    int value = 0;

    if(numeric == NULL)
        return -1;

    while(i < strlen(numeric))
    {
        if(numeric[i] < '0' || numeric[i] > '9')
            return -1;
        value = value*10+numeric[i]-'0';
        i++;
    }

    return value;
}
