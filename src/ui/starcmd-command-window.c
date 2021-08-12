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

#include "starcmd-command-window.h"
#include "starcmd-main-window.h"
#include "starcmd-image-search-window.h"

/* INTERNAL DECLARATIONS */

static void starcmd_command_window_dispose (GObject *object);
static void starcmd_command_window_finalize (GObject *object);

/* GOBJECT DEFINITION */

typedef struct _StarcmdCommandWindowPrivate StarcmdCommandWindowPrivate;

struct _StarcmdCommandWindowPrivate
{
    GtkWidget  *btn_cancel;
    GtkWidget  *btn_save;
    GtkWidget  *btn_browse;
    GtkWidget  *btn_download;
    GtkWidget  *lbl_id;
    GtkWidget  *buffer_name;
    GtkWidget  *buffer_platform;
    GtkWidget  *buffer_os;
    GtkWidget  *buffer_description;
    GtkWidget  *buffer_commands;
    GtkWidget  *buffer_examples;
    GtkWidget  *buffer_references;
    GtkWidget  *buffer_tags;
    GtkWidget  *lbl_datemod_info;
    GtkWidget  *dialog_success;
    const char *name_default;
    const char *name;
    const char *platform;
    const char *os;
    const char *description;
    const char *commands;
    const char *examples;
    const char *references;
    const char *tags;
    const char *datemod;
    const char *icon;
};

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdCommandWindow, starcmd_command_window, GTK_TYPE_WINDOW);

/* CONSTANTS */

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-command-window.glade";

/* METHOD DEFINITIONS */

static void
starcmd_command_window_class_init (StarcmdCommandWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, lbl_id);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_name);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_platform);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_os);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_description);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_commands);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_examples);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_references);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, buffer_tags);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, lbl_datemod_info);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, dialog_success);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), StarcmdCommandWindow, btn_save);
}

static void
starcmd_command_window_init (StarcmdCommandWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}

static void
starcmd_command_window_dispose (GObject *object)
{
    StarcmdCommandWindow *self = STARCMD_COMMAND_WINDOW (object);

    G_OBJECT_CLASS (starcmd_command_window_parent_class)->dispose (object);
}

static void
starcmd_command_window_finalize (GObject *object)
{
    StarcmdCommandWindow *self = STARCMD_COMMAND_WINDOW (object);

    G_OBJECT_CLASS (starcmd_command_window_parent_class)->finalize (object);
}

static gboolean 
is_command_changed (StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);

    // Minimum required fields
    if (priv->name == NULL || strlen (priv->name) == 0)
        return FALSE;

    if (priv->platform == NULL || strlen (priv->platform) == 0)
        return FALSE;

    if (priv->commands == NULL || strlen (priv->commands) == 0)
        return FALSE;

    return TRUE;
}

/* CALLBACK HANDLERS */

void
on_btn_save_clicked (GtkButton *btn, StarcmdCommandWindow *self)
{
    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    // Connect to database
    if (sqlite3_open ("../data/starcmd-demo.db", &db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return;
    }

    if ( (err = save_command (&self, db)) != 0)
    {
        fprintf (stderr, "Cannot save command\n");
        sqlite3_close (db);
        return;
    }
    sqlite3_close (db);

    StarcmdCommandWindowPrivate *priv;
    priv = starcmd_command_window_get_instance_private (self);
    gtk_widget_show (GTK_WIDGET (priv->dialog_success));
}

void
on_btn_cancel_clicked (GtkButton *btn, StarcmdCommandWindow *self)
{
    gtk_widget_destroy (GTK_WIDGET (self));
}

void
on_dialog_success_response (GtkDialog *dialog, gint response_id, StarcmdCommandWindow *self)
{
    gtk_widget_hide (GTK_WIDGET (dialog));
    gtk_widget_destroy (GTK_WIDGET (self));
}

void
on_btn_delete_clicked (GtkButton *btn, gpointer user_data)
{
    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    int id = atoi (gtk_label_get_text (GTK_LABEL (user_data)));  /* user data passed in is the value of lbl_id */

    // Connect to database
    if (sqlite3_open ("../data/starcmd-demo.db", &db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return;
    }

    //err = delete_command (db, id);
    sqlite3_close (db);
}

void
on_btn_browse_clicked (GtkButton *btn)
{
    printf ("browse btn clicked\n");
}

void
on_btn_download_clicked (GtkButton *btn)
{
    StarcmdImageSearchWindow *win = starcmd_image_search_window_new ();
    gtk_window_present (GTK_WINDOW (win));
}

void
on_textentry_name_changed (GtkEditable *editable, StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (priv->buffer_name);
    priv->name = gtk_entry_buffer_get_text (buffer);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_textentry_platform_changed (GtkEditable *editable, StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (priv->buffer_platform);
    priv->platform = gtk_entry_buffer_get_text (buffer);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_textentry_os_changed (GtkEditable *editable, StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (priv->buffer_os);
    priv->os = gtk_entry_buffer_get_text (buffer);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_buffer_description_changed (GtkTextBuffer *textbuffer, StarcmdCommandWindow *self)
{
    GtkTextIter                  begin, end;
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    priv->description = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_buffer_commands_changed (GtkTextBuffer *textbuffer, StarcmdCommandWindow *self)
{
    GtkTextIter                  begin, end;
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    priv->commands = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_buffer_examples_changed (GtkTextBuffer *textbuffer, StarcmdCommandWindow *self)
{
    GtkTextIter                  begin, end;
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    priv->examples = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_buffer_references_changed (GtkTextBuffer *textbuffer, StarcmdCommandWindow *self)
{
    GtkTextIter                  begin, end;
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    priv->references = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_textentry_tags_changed (GtkEditable *editable, StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);

    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (priv->buffer_tags);
    priv->tags = gtk_entry_buffer_get_text (buffer);

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

void
on_btn_browse_file_set (GtkFileChooserButton *btn, StarcmdCommandWindow *self)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (self);

    const char *f_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (btn));
    priv->icon = f_name;

    gtk_widget_set_sensitive (priv->btn_save, is_command_changed (self));
}

/* HELPER METHODS */

int
load_command (StarcmdCommandWindow *self, sqlite3 *db)
{
    int rc;
    sqlite3_stmt *res;

    char *sql = "SELECT name, platform, os, description, command, examples, refs, tags, datemod "
                "FROM commands "
                "WHERE name = ?";

    if ( (rc = sqlite3_prepare_v2 (db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return 1;
    }

    StarcmdCommandWindowPrivate *priv = starcmd_command_window_get_instance_private (self);

    sqlite3_bind_text (res, 1, priv->name_default, strlen (priv->name_default), SQLITE_TRANSIENT);

    int step;
    if ( (step = sqlite3_step (res)) == SQLITE_ROW)
    {
        const gchar *name = (const gchar *) sqlite3_column_text (res, 0);
        const gchar *platform = (const gchar *) sqlite3_column_text (res, 1);
        const gchar *os = (const gchar *) sqlite3_column_text (res, 2);
        const gchar *description = (const gchar *) sqlite3_column_text (res, 3);
        const gchar *commands = (const gchar *) sqlite3_column_text (res, 4);
        const gchar *examples = (const gchar *) sqlite3_column_text (res, 5);
        const gchar *references = (const gchar *) sqlite3_column_text (res, 6);
        const gchar *tags = (const gchar *) sqlite3_column_text (res, 7);
        const gchar *datemod = (const gchar *) sqlite3_column_text (res, 8);

        gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_name), name, strlen (name));
        gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_platform), platform, strlen (platform)); 
        gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_os), os, strlen (os)); 
        gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_description), commands, strlen (commands));
        gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_commands), commands, strlen (commands)); 
        gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_examples), examples, strlen (examples)); 
        gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_references), references, strlen (references)); 
        gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_tags), tags, strlen (tags)); 
        gtk_label_set_text (GTK_LABEL (priv->lbl_datemod_info), datemod);
    }
    sqlite3_finalize (res);

    return 0;
}

int
save_command (StarcmdCommandWindow **self, sqlite3 *db)
{
    StarcmdCommandWindowPrivate *priv;

    priv = starcmd_command_window_get_instance_private (*self);

    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    if (priv->name_default != NULL)
        sql = "UPDATE commands " 
              "SET name = ?, platform = ?, os = ?, description = ?, command = ?, examples = ?, refs = ?, tags = ?, datemod = ?, icon = ? "
              "WHERE name = ?";
    else
        sql = "INSERT INTO COMMANDS (name, platform, os, description, command, examples, refs, tags, datemod, icon)"
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    if ( (rc = sqlite3_prepare_v2 (db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return 1;
    }

    char datemod[256] = {0};
    time_t rawtime = time (NULL);
    if (rawtime == -1)
    {
        printf ("Failed to get date time: {time()}");
        return 1;
    }
    struct tm *ptm = localtime (&rawtime);
    if (ptm == NULL)
    {
        printf ("Failed to get date time: {localtime()}");
        return 1;
    }
    strftime (datemod, 256, "%d/%m/%Y %T", ptm);

    if (priv->os == NULL)
        priv->os = " ";

    if (priv->description == NULL)
        priv->description = " ";

    if (priv->examples == NULL)
        priv->examples = " ";

    if (priv->references == NULL)
        priv->references = " ";

    if (priv->tags == NULL)
        priv->tags = " ";

    if (priv->datemod == NULL)
        priv->datemod = " ";

    if (priv->icon == NULL)
        priv->icon = " ";

    sqlite3_bind_text (res, 1, priv->name, strlen (priv->name), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 2, priv->platform, strlen (priv->platform), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 3, priv->os, strlen (priv->os), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 4, priv->description, strlen (priv->description), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 5, priv->commands, strlen (priv->commands), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 6, priv->examples, strlen (priv->examples), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 7, priv->references, strlen (priv->references), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 8, priv->tags, strlen (priv->tags), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 9, datemod, 256, SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 10, priv->icon, strlen (priv->icon), SQLITE_TRANSIENT);

    if (priv->name_default != NULL)
        sqlite3_bind_text (res, 11, priv->name_default, strlen (priv->name_default), SQLITE_TRANSIENT);

    sqlite3_step (res);
    sqlite3_finalize (res);

    return 0;
}

int
edit_command (void)
{
    return 0;
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdCommandWindow *
starcmd_command_window_new (void)
{
    return g_object_new (STARCMD_TYPE_COMMAND_WINDOW, NULL);
}

void
starcmd_command_window_populate_widgets (StarcmdCommandWindow *self, char *name)
{
    StarcmdCommandWindowPrivate *priv = starcmd_command_window_get_instance_private (self);
    priv->name_default = name;

    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    // Connect to database
    if (sqlite3_open ("../data/starcmd-demo.db", &db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return;
    }

    err = load_command (self, db);
    sqlite3_close (db);
}

