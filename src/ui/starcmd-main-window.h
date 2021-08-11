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

#include <sqlite3/sqlite3.h>

#include "../starcmd-app.h"

G_BEGIN_DECLS

#define STARCMD_TYPE_MAIN_WINDOW (starcmd_main_window_get_type ())

G_DECLARE_FINAL_TYPE (StarcmdMainWindow, starcmd_main_window, STARCMD, MAIN_WINDOW, GtkApplicationWindow)

StarcmdMainWindow *starcmd_main_window_new (StarcmdApp *app);

/* TREESTORE OBJECTS */

typedef enum
{
    KEY,
    VALUE,
    LAST_COLUMN
} ColumnType;

typedef enum
{
    KEY_0,
    KEY_NAME,
    KEY_PLATFORM,
    KEY_OS,
    KEY_DESC,
    KEY_CMDS,
    KEY_EXAMPLES,
    KEY_REFS,
    KEY_TAGS,
    KEY_DATEMOD,
    KEY_ICON,
    KEY_LAST_ROW
} RowType;

static const char *const keys[] = {
    [KEY_NAME]     = "Name",
    [KEY_PLATFORM] = "Platform",
    [KEY_OS]       = "Operating System",
    [KEY_DESC]     = "Description",
    [KEY_CMDS]     = "Command(s)",
    [KEY_EXAMPLES] = "Example(s)",
    [KEY_REFS]     = "Reference(s)",
    [KEY_TAGS]     = "Tag(s)",
    [KEY_DATEMOD]  = "Last Modified",
    [KEY_ICON]     = "Icon Path"
};

/* MAIN WINDOW PROTOTYPES */

static void starcmd_main_window_populate_widgets (StarcmdMainWindow *self);

/* TREESTORE PROTOTYPES */

static int  open_commands                        (sqlite3 **db);
static int  load_commands                        (StarcmdMainWindow *self);
static int  delete_command                       (StarcmdMainWindow *self, char *name);

/* CALLBACK PROTOTYPES */

void        on_tvsel_changed                     (GtkTreeSelection *treeselection, gpointer user_data);
void        on_toolbtn_new_clicked               (GtkToolButton *toolbutton, gpointer user_data);
void        on_toolbtn_edit_clicked              (GtkToolButton *toolbutton, gpointer user_data);
void        on_toolbtn_delete_clicked            (GtkToolButton *toolbutton, gpointer user_data);
void        on_dialog_delete_command_response    (GtkDialog *dialog, gint response_id, gpointer user_data);

G_END_DECLS

#endif /* STARCMD_MAIN_WINDOW_H */
