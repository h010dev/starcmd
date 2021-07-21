#include <gtk/gtk.h>

#include "starcmd-app.h"
#include "starcmd-main-window.h"
#include "starcmd-command-view.h"

struct _StarcmdMainWindow
{
    GtkApplicationWindow parent;
};

typedef struct _StarcmdMainWindowPrivate StarcmdMainWindowPrivate;

struct _StarcmdMainWindowPrivate
{
    GtkWidget *grid_commands;
};

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdMainWindow, starcmd_main_window, GTK_TYPE_APPLICATION_WINDOW);

static void
starcmd_main_window_populate_widgets (StarcmdMainWindow *self)
{
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);

    for (int i = 0; i < 10; i++) {
        gtk_grid_insert_row (GTK_GRID (priv->grid_commands), i + 1);
        gtk_grid_attach (GTK_GRID (priv->grid_commands), starcmd_command_view_new (), 1, i + 1, 1, 1);
    }
    
    gtk_widget_show_all (priv->grid_commands);
}

static void
starcmd_main_window_class_init (StarcmdMainWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), 
                                                 "/org/h010dev/starcmd/starcmd-main-window.glade");

    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, grid_commands);
}

static void
starcmd_main_window_init (StarcmdMainWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    starcmd_main_window_populate_widgets (self);
}

StarcmdMainWindow *
starcmd_main_window_new (StarcmdApp *app)
{
    return g_object_new (STARCMD_TYPE_MAIN_WINDOW, "application", app, NULL);
}
