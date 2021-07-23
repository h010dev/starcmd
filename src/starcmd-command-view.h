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

// https://github.com/elboulangero/goodvibes/blob/master/src/ui/gv-playlist-view.h
#ifndef STARCMD_COMMAND_VIEW_H
#define STARCMD_COMMAND_VIEW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_COMMAND_VIEW (starcmd_command_view_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdCommandView, starcmd_command_view, STARCMD, COMMAND_VIEW, GtkGrid)

GtkWidget      *starcmd_command_view_new (void);

/* GETTERS AND SETTERS */
const gchar    *starcmd_command_view_get_name     (StarcmdCommandView *self);
void            starcmd_command_view_set_name     (StarcmdCommandView *self,
                                                   const gchar        *name);

const gchar    *starcmd_command_view_get_platform (StarcmdCommandView *self);
void            starcmd_command_view_set_platform (StarcmdCommandView *self,
                                                   const gchar        *name);

const gchar    *starcmd_command_view_get_os       (StarcmdCommandView *self);
void            starcmd_command_view_set_os       (StarcmdCommandView *self,
                                                   const gchar        *os);

const gchar    *starcmd_command_view_get_desc     (StarcmdCommandView *self);
void            starcmd_command_view_set_desc     (StarcmdCommandView *self,
                                                   const gchar        *desc);

const gchar    *starcmd_command_view_get_cmds     (StarcmdCommandView *self);
void            starcmd_command_view_set_cmds     (StarcmdCommandView *self,
                                                   const gchar        *cmds);

const gchar    *starcmd_command_view_get_exs      (StarcmdCommandView *self);
void            starcmd_command_view_set_exs      (StarcmdCommandView *self,
                                                   const gchar        *exs);

const gchar    *starcmd_command_view_get_refs     (StarcmdCommandView *self);
void            starcmd_command_view_set_refs     (StarcmdCommandView *self,
                                                   const gchar        *refs);

const gchar    *starcmd_command_view_get_datemod  (StarcmdCommandView *self);
void            starcmd_command_view_set_datemod  (StarcmdCommandView *self,
                                                   const gchar        *datemod);

const gchar    *starcmd_command_view_get_icon     (StarcmdCommandView *self);
void            starcmd_command_view_set_icon     (StarcmdCommandView *self,
                                                   const gchar        *icon);

const gboolean  starcmd_command_view_get_fav      (StarcmdCommandView *self);
void            starcmd_command_view_set_fav      (StarcmdCommandView *self,
                                                   const gboolean      fav);

G_END_DECLS

#endif /* STARCMD_COMMAND_VIEW_H */
