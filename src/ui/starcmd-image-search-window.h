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

#ifndef STARCMD_IMAGE_SEARCH_WINDOW_H
#define STARCMD_IMAGE_SEARCH_WINDOW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_IMAGE_SEARCH_WINDOW (starcmd_image_search_window_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdImageSearchWindow, starcmd_image_search_window, STARCMD, IMAGE_SEARCH_WINDOW, GtkWindow)

struct _StarcmdImageSearchWindow
{
    GtkWindow parent;
};

StarcmdImageSearchWindow *starcmd_image_search_window_new (void);

/* CALLBACK HANDLERS */
void on_textentry_img_name_changed (GtkEditable *editable, gpointer user_data);
void on_btn_img_find_clicked       (GtkButton *btn, gpointer user_data);
void on_btn_img_add_clicked        (GtkButton *btn, gpointer user_data);

G_END_DECLS

#endif /* STARCMD_IMAGE_SEARCH_WINDOW_H */
