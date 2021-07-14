#include <gtk/gtk.h>

#include "starcmd.h"
#include "starcmdwin.h"
#include "commandview.h"

struct _StarcmdAppWindow
{
    GtkApplicationWindow parent;
};

typedef struct _StarcmdAppWindowPrivate StarcmdAppWindowPrivate;

struct _StarcmdAppWindowPrivate
{
    GtkWidget *grid_commands;
};

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdAppWindow, starcmd_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
starcmd_app_window_populate_widgets (StarcmdAppWindow *win)
{
    StarcmdAppWindowPrivate *priv = starcmd_app_window_get_instance_private (win);
    GtkWidget *cmd = starcmd_app_command_view_new ();

    gtk_grid_insert_row (GTK_GRID (priv->grid_commands), 1);
    gtk_grid_attach (GTK_GRID (priv->grid_commands), cmd, 1, 1, 1, 1);

    gtk_widget_show_all (priv->grid_commands);
}

static void
starcmd_app_window_init (StarcmdAppWindow *win)
{
    gtk_widget_init_template (GTK_WIDGET (win));
    starcmd_app_window_populate_widgets (win);
}

static void
starcmd_app_window_class_init (StarcmdAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class), "/org/h010dev/starcmd/window.glade");
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), StarcmdAppWindow, grid_commands);
}

StarcmdAppWindow *
starcmd_app_window_new (StarcmdApp *app)
{
    return g_object_new (STARCMD_APP_WINDOW_TYPE, "application", app, NULL);
}
