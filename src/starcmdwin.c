#include <gtk/gtk.h>

#include "starcmd.h"
#include "starcmdwin.h"
// #include "commandview.h"

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
starcmd_app_window_init (StarcmdAppWindow *win)
{

    gtk_widget_init_template (GTK_WIDGET (win));

    // Add single command view item.
    // https://gitlab.gnome.org/GNOME/gtk/-/blob/gtk-3-22/examples/application10/exampleappwin.c
    GtkWidget *cmd;
    GtkBuilder *builder;
    StarcmdAppWindowPrivate *priv;

    priv = starcmd_app_window_get_instance_private (win);

    builder = gtk_builder_new_from_resource ("/org/h010dev/starcmd/command.glade");
    cmd = GTK_WIDGET (gtk_builder_get_object (builder, "command"));
    gtk_grid_insert_row (GTK_GRID (priv->grid_commands), 1);
    gtk_grid_attach (GTK_GRID (priv->grid_commands), cmd, 1, 1, 1, 1);
    g_object_unref (builder);

    gtk_widget_show_all (GTK_WIDGET (win));

    // Not working at the moment.
    // GtkWidget *btn;
    // cmd = starcmd_app_command_view_new ();
    // btn = gtk_button_new_with_label ("HELLO");
    // gtk_grid_attach (GTK_GRID (priv->grid_commands), btn, 1, 1, 1, 1);

    // Add second command view item.
    // GtkWidget *cmd1;
    // builder = gtk_builder_new_from_resource ("/org/h010dev/starcmd/command.glade");
    // cmd1 = GTK_WIDGET (gtk_builder_get_object (builder, "command"));
    // gtk_grid_insert_row (GTK_GRID (priv->grid_commands), 2);
    // gtk_grid_attach (GTK_GRID (priv->grid_commands), cmd1, 1, 2, 1, 1);
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
