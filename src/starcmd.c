#include <gtk/gtk.h>

#include "starcmd.h"
#include "starcmdwin.h"

struct _StarcmdApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE (StarcmdApp, starcmd_app, GTK_TYPE_APPLICATION);

static void
starcmd_app_init (StarcmdApp *app)
{
}

static void
starcmd_app_activate (GApplication *app)
{
    StarcmdAppWindow *win;

    win = starcmd_app_window_new (STARCMD_APP (app));
    gtk_window_present (GTK_WINDOW (win));
}

static void
starcmd_app_class_init (StarcmdAppClass *class)
{
    G_APPLICATION_CLASS (class)->activate = starcmd_app_activate;
}

StarcmdApp *
starcmd_app_new (void)
{
    return g_object_new (STARCMD_APP_TYPE,
                         "application-id", "org.h010dev.starcmd",
                         "flags", G_APPLICATION_HANDLES_OPEN,
                         NULL);
}
