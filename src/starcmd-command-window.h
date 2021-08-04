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

#ifndef STARCMD_COMMAND_WINDOW_H
#define STARCMD_COMMAND_WINDOW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_COMMAND_WINDOW (starcmd_command_window_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdCommandWindow, starcmd_command_window, STARCMD, COMMAND_Window, GtkWindow)

StarcmdCommandWindow *starcmd_command_window_new (void);

G_END_DECLS

#endif /* STARCMD_COMMAND_WINDOW_H */