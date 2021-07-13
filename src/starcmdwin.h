#ifndef __STARCMDAPPWIN_H
#define __STARCMDAPPWIN_H

#include <gtk/gtk.h>
#include "starcmd.h"


#define STARCMD_APP_WINDOW_TYPE (starcmd_app_window_get_type ())
G_DECLARE_FINAL_TYPE (StarcmdAppWindow, starcmd_app_window, STARCMD, APP_WINDOW, GtkApplicationWindow)


StarcmdAppWindow *starcmd_app_window_new (StarcmdApp *app);


#endif /* __STARCMDAPPWIN_H */
