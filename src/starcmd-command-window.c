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

#include "starcmd-command-window.h"

/* GOBJECT DEFINITION */

struct _StarcmdCommandWindow
{
    GtkWindow parent;
};

typedef struct _StarcmdCommandWindowPrivate StarcmdCommandWindowPrivate;

struct _StarcmdCommandWindowPrivate
{
    GtkWidget *btn_cancel;
    GtkWidget *btn_save;
    GtkWidget *btn_browse;
    GtkWidget *btn_download;
};

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-command-window.glade";

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdCommandWindow, starcmd_command_window, GTK_TYPE_WINDOW);

void
on_btn_cancel_clicked (GtkButton *btn)
{
    printf("cancel");
}

void
on_btn_save_clicked (GtkButton *btn)
{
    printf("save");
}

void
on_btn_browse_clicked (GtkButton *btn)
{
    printf("browse");
}

void
on_btn_download_clicked (GtkButton *btn)
{
    printf("download");
}

static void
starcmd_command_window_class_init (StarcmdCommandWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, btn_cancel);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, btn_save);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, btn_browse);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, btn_download);
}

static void
starcmd_command_window_init (StarcmdCommandWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    StarcmdCommandWindowPrivate *priv = starcmd_command_window_get_instance_private (self);
    g_signal_connect (priv->btn_cancel, "clicked", G_CALLBACK (on_btn_cancel_clicked), NULL);
    g_signal_connect (priv->btn_save, "clicked", G_CALLBACK (on_btn_save_clicked), NULL);
    g_signal_connect (priv->btn_browse, "clicked", G_CALLBACK (on_btn_browse_clicked), NULL);
    g_signal_connect (priv->btn_download, "clicked", G_CALLBACK (on_btn_download_clicked), NULL);
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdCommandWindow *
starcmd_command_window_new (void)
{
    return g_object_new (STARCMD_TYPE_COMMAND_WINDOW, NULL);
}
