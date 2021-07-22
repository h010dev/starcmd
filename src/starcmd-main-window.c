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
#include "starcmd-main-window.h"
#include "starcmd-command-view.h"

/* GOBJECT DEFINITION */

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

/* METHOD DEFINITIONS */

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

/* PUBLIC METHOD DEFINITIONS */

StarcmdMainWindow *
starcmd_main_window_new (StarcmdApp *app)
{
    return g_object_new (STARCMD_TYPE_MAIN_WINDOW, "application", app, NULL);
}
