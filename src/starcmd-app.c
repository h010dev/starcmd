#include <gtk/gtk.h>

#include "starcmd-app.h"
#include "starcmd-main-window.h"

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
    StarcmdMainWindow *win;

    win = starcmd_main_window_new (STARCMD_APP (app));
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
    return g_object_new (STARCMD_TYPE_APP,
                         "application-id", "org.h010dev.starcmd",
                         "flags", G_APPLICATION_HANDLES_OPEN,
                         NULL);
}
