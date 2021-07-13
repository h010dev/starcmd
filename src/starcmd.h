#ifndef __STARCMDAPP_H
#define __STARCMDAPP_H

#include <gtk/gtk.h>


#define STARCMD_APP_TYPE (starcmd_app_get_type ())
G_DECLARE_FINAL_TYPE (StarcmdApp, starcmd_app, STARCMD, APP, GtkApplication)

StarcmdApp *starcmd_app_new (void);

#endif /* __STARCMDAPP_H */
