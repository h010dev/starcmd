// https://github.com/elboulangero/goodvibes/blob/master/src/ui/gv-playlist-view.h
#ifndef STARCMD_COMMAND_VIEW_H
#define STARCMD_COMMAND_VIEW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_COMMAND_VIEW (starcmd_command_view_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdCommandView, starcmd_command_view, STARCMD, COMMAND_VIEW, GtkGrid)

GtkWidget *starcmd_command_view_new (void);

G_END_DECLS

#endif /* STARCMD_COMMAND_VIEW_H */
