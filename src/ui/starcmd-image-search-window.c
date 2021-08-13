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

#include "starcmd-image-search-window.h"
#include "../core/starcmd-image-client.h"

/* GOBJECT DEFINITION */

typedef struct _StarcmdImageSearchWindowPrivate StarcmdImageSearchWindowPrivate;

struct _StarcmdImageSearchWindowPrivate
{
    GtkWidget  *img_result;
    const char *search_query;
    const char *result_path;
} _StarcmdImageSearchWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdImageSearchWindow, starcmd_image_search_window, GTK_TYPE_WINDOW);

/* CONSTANTS */

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-image-search-window.glade";

/* METHOD DEFINITIONS */

static void
starcmd_image_search_window_class_init (StarcmdImageSearchWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdImageSearchWindow, img_result);
}

static void
starcmd_image_search_window_init (StarcmdImageSearchWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}

/* CALLBACK HANDLERS */

void
on_textentry_img_name_changed (GtkEditable *editable, gpointer user_data)
{
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (user_data);
    _StarcmdImageSearchWindowPrivate.search_query = gtk_entry_buffer_get_text (buffer);
}

void
on_btn_img_find_clicked (GtkButton *btn, gpointer user_data)
{
    (void) user_data;
    int err;
    //download_image ("python");
}

void
on_btn_img_add_clicked (GtkButton *btn, gpointer user_data)
{
    (void) user_data;
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdImageSearchWindow *
starcmd_image_search_window_new (void)
{
    return g_object_new (STARCMD_TYPE_IMAGE_SEARCH_WINDOW, NULL);
}
