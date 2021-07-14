// https://github.com/elboulangero/goodvibes/blob/master/src/ui/gv-playlist-view.h
#include <glib-object.h>
#include <gtk/gtk.h>

#define STARCMD_APP_COMMAND_VIEW_TYPE (starcmd_app_command_view_get_type ())
G_DECLARE_FINAL_TYPE (StarcmdAppCommandView, starcmd_app_command_view, STARCMD, COMMAND_VIEW, GtkGrid)

GtkWidget *starcmd_app_command_view_new (void);
