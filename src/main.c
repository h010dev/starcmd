#include <gtk/gtk.h>

#include "starcmd.h"

int
main (int argc, char *argv[])
{
    return g_application_run (G_APPLICATION (starcmd_app_new ()), argc, argv); 
}
