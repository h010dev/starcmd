/*
 * Starcmd Command Browser
 *
 * Copyright © 2021 Mohamed Al-Hussein <mohamed.n.al.hussein@gmail.com>
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "starcmd-app.h"
#include "ui/starcmd-main-window.h"

/* GOBJECT DEFINITION */

struct _StarcmdApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE (StarcmdApp, starcmd_app, GTK_TYPE_APPLICATION);

/* METHOD DEFINITIONS */

static void
starcmd_app_activate (GApplication *app)
{
    StarcmdMainWindow *win;

    win = starcmd_main_window_new (STARCMD_APP (app));
    gtk_window_present (GTK_WINDOW (win));
}

static void
starcmd_app_class_init (StarcmdAppClass *klass)
{
    G_APPLICATION_CLASS (klass)->activate = starcmd_app_activate;
}

static void
starcmd_app_init (StarcmdApp *self)
{
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdApp *
starcmd_app_new (void)
{
    return g_object_new (STARCMD_TYPE_APP,
                         "application-id", "org.h010dev.starcmd",
                         "flags", G_APPLICATION_HANDLES_OPEN,
                         NULL);
}
