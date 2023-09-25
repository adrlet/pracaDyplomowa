#ifndef WIDGETMANAGMENT_H_INCLUDED
#define WIDGETMANAGMENT_H_INCLUDED

#include <gtk/gtk.h>

#define functionalities_pointer_new(funcPoint) do { \
    funcPoint = (struct functionalities_pointer*)malloc(sizeof(struct functionalities_pointer)); \
    funcPoint->data = NULL; \
    funcPoint->functions = NULL; \
} while(0)

struct functionalities_pointer
{
    void * data; // 8
    void (**functions)(GtkButton*, gpointer); // 16
    //GCallback * functions; // 16
};

// Oczyszczanie kontenera z potomków
void empty_box(GtkWidget * box);

// Przeszuiwanie kontenera
GtkWidget * get_child_of_type(GtkWidget * box, GType type, unsigned char sibling);

GtkWidget * get_parent_of_type(GtkWidget * child, GType type);

void humanReadableBytes(unsigned long long fileSize, char * inputEntry_cbBuffer);

long long unsigned int getFileSize(const char * filePath);

char * getFileName(const char * filePath);

int skipSeparator(const char * record, int skipped, const char * separator);

int string_to_int(const char * numeric);

#endif // WIDGETMANAGMENT_H_INCLUDED
