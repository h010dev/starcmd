#ifndef STARCMD_MAIN_WINDOW_H
#define STARCMD_MAIN_WINDOW_H

#include <gtk/gtk.h>
#include "starcmd-app.h"

G_BEGIN_DECLS

#define STARCMD_TYPE_MAIN_WINDOW (starcmd_main_window_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdMainWindow, starcmd_main_window, STARCMD, MAIN_WINDOW, GtkApplicationWindow)

StarcmdMainWindow *starcmd_main_window_new (StarcmdApp *app);

G_END_DECLS

#endif /* STARCMD_MAIN_WINDOW_H */
