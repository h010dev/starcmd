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

#ifndef STARCMD_MAIN_WINDOW_H
#define STARCMD_MAIN_WINDOW_H

#include <gtk/gtk.h>
#include "starcmd-app.h"

G_BEGIN_DECLS

#define STARCMD_TYPE_MAIN_WINDOW (starcmd_main_window_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdMainWindow, starcmd_main_window, STARCMD, MAIN_WINDOW, GtkApplicationWindow)

StarcmdMainWindow *starcmd_main_window_new (StarcmdApp *app);

G_END_DECLS

#endif /* STARCMD_MAIN_WINDOW_H */
