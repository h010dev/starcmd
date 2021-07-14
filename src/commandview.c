#include <gtk/gtk.h>

#include "commandview.h"

struct _StarcmdAppCommandView
{
    GtkApplicationWindow parent;
};

// https://github.com/elboulangero/goodvibes/blob/master/src/ui/gv-playlist-view.c
G_DEFINE_TYPE (StarcmdAppCommandView, starcmd_app_command_view, GTK_TYPE_GRID);

static void
starcmd_app_command_view_init (StarcmdAppCommandView *view)
{
    gtk_widget_init_template (GTK_WIDGET (view));
}

static void
starcmd_app_command_view_class_init (StarcmdAppCommandViewClass *class)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class), "/org/h010dev/starcmd/command.glade");
}

GtkWidget *
starcmd_app_command_view_new (void)
{

    /*
    GtkBuilder *builder;
    GtkWidget *cmd;
    builder = gtk_builder_new_from_resource ("/org/h010dev/starcmd/command.glade");
    cmd = GTK_WIDGET (gtk_builder_get_object (builder, "command"));
    g_object_unref (builder);
    return cmd;
    */
    return g_object_new (STARCMD_APP_COMMAND_VIEW_TYPE, NULL);
}
