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
#include <sqlite3/sqlite3.h>

#include "../starcmd-app.h"
#include "starcmd-main-window.h"
#include "starcmd-command-view.h"
#include "starcmd-command-window.h"

/* GOBJECT DEFINITION */

struct _StarcmdMainWindow
{
    GtkApplicationWindow parent;
};

typedef struct _StarcmdMainWindowPrivate StarcmdMainWindowPrivate;

struct _StarcmdMainWindowPrivate
{
    GtkTreeStore      *tstore_commands;
    GtkTreeView       *tv_commands;
    GtkTreeViewColumn *tvcol_key;
    GtkCellRenderer   *tvrow_key;
    GtkTreeViewColumn *tvcol_value;
    GtkCellRenderer   *tvrow_value;
    GtkTreeSelection  *tvsel;
    GtkToolButton     *toolbtn_edit;
    GtkToolButton     *toolbtn_delete;
    GtkToolButton     *toolbtn_star;
    GtkDialog         *dialog_delete_command;
};

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdMainWindow, starcmd_main_window, GTK_TYPE_APPLICATION_WINDOW);

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-main-window.glade";
static const char  *f_db          = "../data/starcmd-demo.db";

/* MAIN WINDOW METHOD DEFINITIONS */

static void
starcmd_main_window_class_init (StarcmdMainWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tstore_commands);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tv_commands);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tvcol_key);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tvrow_key);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tvcol_value);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tvrow_value);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, tvsel);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, toolbtn_edit);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, toolbtn_delete);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, toolbtn_star);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdMainWindow, dialog_delete_command);
}

static void
starcmd_main_window_init (StarcmdMainWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    starcmd_main_window_populate_widgets (self);
}

static void
starcmd_main_window_populate_widgets (StarcmdMainWindow *self)
{
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);
    int                       err;

    if ( (err = load_commands (self)) != 0)
    {
        fprintf (stderr, "Load commands failed\n");
        return;
    }

    gtk_widget_show_all (GTK_WIDGET (priv->tv_commands));
}

/* CALLBACK HANDLERS */

void
on_tvsel_changed (GtkTreeSelection *treeselection, gpointer user_data)
{
    gchar                    *value;
    GtkTreeIter               iter;
    GtkTreeModel             *model;
    StarcmdMainWindow        *self = STARCMD_MAIN_WINDOW (user_data);
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self); 

    if (gtk_tree_selection_get_selected (treeselection, &model, &iter) == FALSE)
    {
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_edit), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_delete), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_star), FALSE);
        return;
    }

    // Only handle selection of top level parent (Key = Name)
    gtk_tree_model_get (model, &iter, KEY, &value, -1);
    if (strcmp (value, keys[KEY_NAME]) != 0)
    {
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_edit), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_delete), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_star), FALSE);
        return;
    }

    gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_edit), TRUE);
    gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_delete), TRUE);
    gtk_widget_set_sensitive (GTK_WIDGET (priv->toolbtn_star), TRUE);
}

void
on_toolbtn_new_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
    StarcmdCommandWindow *win = starcmd_command_window_new ();
    gtk_window_present (GTK_WINDOW (win));
}

void
on_toolbtn_edit_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
    gchar            *value;
    GtkTreeIter       iter;
    GtkTreeModel     *model;
    GtkTreeSelection *treeselection = GTK_TREE_SELECTION (user_data);

    if (gtk_tree_selection_get_selected (treeselection, &model, &iter) == FALSE)
        return;

    gtk_tree_model_get (model, &iter, VALUE, &value, -1);

    StarcmdCommandWindow *win = starcmd_command_window_new ();
    gtk_window_present (GTK_WINDOW (win));
    starcmd_command_window_populate_widgets (win, value);
}

void
on_toolbtn_delete_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
    StarcmdMainWindow *self = STARCMD_MAIN_WINDOW (user_data);
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);

    gtk_widget_show (GTK_WIDGET (priv->dialog_delete_command));
}

void
on_dialog_delete_command_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    StarcmdMainWindow *self = STARCMD_MAIN_WINDOW (user_data);
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);
    gtk_widget_hide (GTK_WIDGET (priv->dialog_delete_command));

    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;

    gtk_tree_selection_get_selected (priv->tvsel, &model, &iter);
    gtk_tree_model_get (model, &iter, VALUE, &value, -1);

    switch (response_id)
    {
        case GTK_RESPONSE_OK:
            delete_command (self, value);
            gtk_tree_store_remove (priv->tstore_commands, &iter);
            break;
        case GTK_RESPONSE_CANCEL:
            break;
        default:
            break;
    }
}

/* TREESTORE METHOD DEFINITIONS */

static int
open_commands (sqlite3 **db)
{
    if (sqlite3_open (f_db, db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (*db));
        sqlite3_close (*db);
        return 1;
    }

    return 0;
}

static int
load_commands (StarcmdMainWindow *self)
{
    int err; 
    sqlite3 *db;

    if ( (err = open_commands (&db)) != 0)
        return 1;

    int                       rc;
    char                     *sql;
    sqlite3_stmt             *res;
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);

    sql = "SELECT id, name, platform, os, description, command, examples, refs, tags, datemod, icon, fav "
          "FROM commands "
          "ORDER BY name ASC;";

    if ( (rc = sqlite3_prepare_v2 (db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (db));
        return 1;
    }

    // Populate treestore with data from command database.
    GtkTreeIter iter;
    GtkTreeIter iter_child;
    gtk_tree_view_column_add_attribute (priv->tvcol_key, priv->tvrow_key, "text", KEY);
    gtk_tree_view_column_add_attribute (priv->tvcol_value, priv->tvrow_value, "text", VALUE);

    while (sqlite3_step (res) == SQLITE_ROW)
    {
        gtk_tree_store_append (priv->tstore_commands, &iter, NULL);
        gtk_tree_store_set (priv->tstore_commands, &iter, KEY, keys[KEY_NAME], -1);
        gtk_tree_store_set (priv->tstore_commands, &iter, VALUE, sqlite3_column_text (res, 1), -1);

        for (int i = KEY_PLATFORM; i < KEY_LAST_ROW; i++)
        {
            gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
            gtk_tree_store_set (priv->tstore_commands, &iter_child, KEY, keys[i], -1);
            gtk_tree_store_set (priv->tstore_commands, &iter_child, VALUE, sqlite3_column_text (res, i), -1);
        }
    }
    sqlite3_finalize (res);
    sqlite3_close (db);

    return 0;
}

static int
delete_command (StarcmdMainWindow *self, char *name)
{
    sqlite3 *db;
    int      err;

    if ( (err = open_commands (&db)) != 0 )
        return 1;

    int           rc;
    sqlite3_stmt *res;

    char *sql = "DELETE FROM commands "
                "WHERE name = ?";

    if ( (rc = sqlite3_prepare_v2 (db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return 1;
    }

    sqlite3_bind_text (res, 1, name, strlen (name), SQLITE_TRANSIENT);
    sqlite3_step (res);
    sqlite3_finalize (res);
    sqlite3_close (db);

    return 0;
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdMainWindow *
starcmd_main_window_new (StarcmdApp *app)
{
    return g_object_new (STARCMD_TYPE_MAIN_WINDOW, "application", app, NULL);
}
