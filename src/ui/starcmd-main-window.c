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
};

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-main-window.glade";
gboolean activated = FALSE;

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdMainWindow, starcmd_main_window, GTK_TYPE_APPLICATION_WINDOW);

typedef struct CommandNode
{
    guint                id;
    const char          *name;
    const char          *platform;
    const gchar         *os;
    const gchar         *description;
    const gchar         *command; 
    const gchar         *examples;
    const gchar         *references;
    const gchar         *tags;
    const gchar         *datemod;
    const gchar         *icon_path;
    gboolean             fav;
    struct CommandNode  *next;
} CommandNode_t;

/* METHOD DEFINITIONS */

void on_menubtn_new_activate (GtkMenuItem *m);

static int
load_commands (StarcmdMainWindow *self, sqlite3 *db, CommandNode_t **commands)
{
    int           rc;
    sqlite3_stmt *res;
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);

    if ( (rc = sqlite3_prepare_v2 (db, "SELECT * FROM commands ORDER BY name DESC;", -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return 1;
    }

    GtkTreeIter iter;
    GtkTreeIter iter_child;
    gtk_tree_view_column_add_attribute (priv->tvcol_key, priv->tvrow_key, "text", 0);
    gtk_tree_view_column_add_attribute (priv->tvcol_value, priv->tvrow_value, "text", 1);

    // Iterate through table rows and create command widget with field data from each column.
    while (sqlite3_step (res) == SQLITE_ROW)
    {
        CommandNode_t *new_node = NULL; 
        if ( (new_node = (CommandNode_t *) malloc (sizeof (CommandNode_t))) == NULL)
            return 1;

        int columns;
        if ( (columns = sqlite3_column_count (res)) < 11)          /* table must have 11 cols min */
        {
            fprintf(stderr, "Column count failed\n");
            return 1;
        }

        // Create a new CommandView object and populate its fields
        GtkWidget *cmd = starcmd_command_view_new ();
        for (int i = 0; i < columns; i++)
        {
            const char *col_name = sqlite3_column_name (res, i);
            if (strcmp (col_name, "id") == 0)
                new_node->id = sqlite3_column_int (res, i);
            else if (strcmp (col_name, "name") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter, NULL);
                gtk_tree_store_set (priv->tstore_commands, &iter, 0, "Name", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->name = (const char *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "platform") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Platform", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->platform = (const char *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "os") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Operating System", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->os = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "description") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Description", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->description = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "command") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Command(s)", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->command = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "examples") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Example(s)", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->examples = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "refs") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Reference(s)", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->references = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "tags") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Tag(s)", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->tags = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "datemod") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Last Modified", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->datemod = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "icon") == 0)
            {
                gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Icon Path", -1);
                gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, sqlite3_column_text (res, i), -1);
            }
                //new_node->icon_path = (const gchar *) sqlite3_column_text (res, i);
            else if (strcmp (col_name, "fav") == 0)
                new_node->fav = (gboolean) sqlite3_column_int (res, i);
            else
            {
                fprintf(stderr, "Invalid column name: %s\n", col_name);
                continue;
            }
            printf("%s = %s\n", col_name, sqlite3_column_text (res, i));
        }
        printf("\n");

        // Insert new command widget at head
        new_node->next = *commands;
        *commands = new_node;
    }

    sqlite3_finalize (res);

    gtk_widget_show_all (GTK_WIDGET (priv->tv_commands));
    return 0;
}

static void
starcmd_main_window_populate_widgets (StarcmdMainWindow *self)
{
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);
    sqlite3                  *db;
    char                     *err_msg = 0;

    // Connect to database
    if (sqlite3_open ("../data/starcmd-demo.db", &db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return;
    }

    // Load commands & create widgets
    CommandNode_t *commands = NULL;
    commands = (CommandNode_t *) malloc (sizeof (CommandNode_t));
    commands->next = NULL;
    int err;
    if ( (err = load_commands (self, db, &commands)) == 1)
    {
        fprintf (stderr, "Load commands failed\n");
        return;
    }

    // Populate main window with command widgets
    /*
    GtkTreeIter iter;
    GtkTreeIter iter_child;
    gtk_tree_view_column_add_attribute (priv->tvcol_key, priv->tvrow_key, "text", 0);
    gtk_tree_view_column_add_attribute (priv->tvcol_value, priv->tvrow_value, "text", 1);
    CommandNode_t *curr = commands;
    while (curr != NULL)
    {
        // Last node has no data
        if (curr->next != NULL)
        {
            gtk_tree_store_append (priv->tstore_commands, &iter, NULL);
            gtk_tree_store_set (priv->tstore_commands, &iter, 0, "Name", -1);
            gtk_tree_store_set (priv->tstore_commands, &iter, 1, curr->name, -1);

            gtk_tree_store_append (priv->tstore_commands, &iter_child, &iter);
            gtk_tree_store_set (priv->tstore_commands, &iter_child, 0, "Platform", -1);
            gtk_tree_store_set (priv->tstore_commands, &iter_child, 1, curr->platform, -1);
        }
        curr = curr->next;
    }
    gtk_widget_show_all (GTK_WIDGET (priv->tv_commands));

    free (commands);
    free (curr);
    */
}

void
on_menubtn_new_activate (GtkMenuItem *m)
{
    if (!activated)
    {
        StarcmdCommandWindow *win = starcmd_command_window_new ();
        gtk_window_present (GTK_WINDOW (win));
        activated = TRUE;
    }
    else
        activated = FALSE;
}

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
}

static void
starcmd_main_window_init (StarcmdMainWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
    StarcmdMainWindowPrivate *priv = starcmd_main_window_get_instance_private (self);
    //g_signal_connect (priv->menubtn_new, "activate", G_CALLBACK (on_menubtn_new_activate), NULL);
    starcmd_main_window_populate_widgets (self);
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdMainWindow *
starcmd_main_window_new (StarcmdApp *app)
{
    return g_object_new (STARCMD_TYPE_MAIN_WINDOW, "application", app, NULL);
}
