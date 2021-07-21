#ifndef __STARCMD_APP_H
#define __STARCMD_APP_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_APP (starcmd_app_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdApp, starcmd_app, STARCMD, APP, GtkApplication)

StarcmdApp *starcmd_app_new (void);

G_END_DECLS

#endif /* __STARCMD_APP_H */
