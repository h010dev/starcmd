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

#ifndef __STARCMD_APP_H
#define __STARCMD_APP_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define STARCMD_TYPE_APP (starcmd_app_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdApp, starcmd_app, STARCMD, APP, GtkApplication)

StarcmdApp *starcmd_app_new (void);

G_END_DECLS

#endif /* __STARCMD_APP_H */
