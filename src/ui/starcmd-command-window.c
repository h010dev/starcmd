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
#include "sqlite3.h"

#include "starcmd-command-window.h"
#include "starcmd-main-window.h"
#include "../core/starcmd-db.h"

/* INTERNAL DECLARATIONS */

static void starcmd_command_window_dispose (StarcmdCommandWindow *self);
static void starcmd_command_window_finalize (StarcmdCommandWindow *self);

/* GOBJECT DEFINITION */

typedef struct _StarcmdCommandWindowPrivate StarcmdCommandWindowPrivate;

struct _StarcmdCommandWindowPrivate
{
    GtkWidget  *btn_save;

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
};

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdCommandWindow, starcmd_command_window, GTK_TYPE_WINDOW);

/* CONSTANTS */

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-command-window.glade";

/* COMMAND WINDOW METHOD DEFINITIONS */

static void
starcmd_command_window_class_init (StarcmdCommandWindowClass *klass)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
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
starcmd_command_window_dispose (StarcmdCommandWindow *self)
{
    G_OBJECT_CLASS (starcmd_command_window_parent_class)->dispose (G_OBJECT (self));
}

static void
starcmd_command_window_finalize (StarcmdCommandWindow *self)
{
    G_OBJECT_CLASS (starcmd_command_window_parent_class)->finalize (G_OBJECT (self));
}

void
starcmd_command_window_populate_widgets (StarcmdCommandWindow *self, char *name)
{
    StarcmdCommandWindowPrivate *priv = starcmd_command_window_get_instance_private (self);
    priv->name_default = name;

    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    if ( (err = starcmd_db_open (&db)) != 0)
        return;

    err = load_command (self, db);
    starcmd_db_close (&db);
}

/* CALLBACK HANDLERS */

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
on_btn_save_clicked (GtkButton *btn, StarcmdCommandWindow *self)
{
    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    // Connect to database
    if ( (err = starcmd_db_open (&db)) != 0 )
        return;

    if ( (err = save_command (&self, db)) != 0)
    {
        fprintf (stderr, "Cannot save command\n");
        starcmd_db_close (&db);
        return;
    }
    starcmd_db_close (&db);

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

/* HELPER METHODS */

gboolean 
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

int
set_timestamp (char datemod[])
{
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

    return 0;
}

/* DATABASE METHODS */

int
load_command (StarcmdCommandWindow *self, sqlite3 *db)
{
    StarcmdCommandWindowPrivate *priv;
    struct CommandData          *data;
    int                          err;

    priv = starcmd_command_window_get_instance_private (self);
    data = malloc (sizeof (struct CommandData));

    if ( (err = starcmd_db_load (&db, (char *) priv->name_default, &data)) != 0)
    {
        starcmd_db_close (&db);
        return 1;
    }
    starcmd_db_close (&db);

    gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_name), data->name, strlen (data->name));
    gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_platform), data->platform, strlen (data->platform)); 
    gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_os), data->os, strlen (data->os)); 
    gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_description), data->description, strlen (data->description));
    gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_commands), data->commands, strlen (data->commands)); 
    gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_examples), data->examples, strlen (data->examples)); 
    gtk_text_buffer_set_text (GTK_TEXT_BUFFER (priv->buffer_references), data->references, strlen (data->references)); 
    gtk_entry_buffer_set_text (GTK_ENTRY_BUFFER (priv->buffer_tags), data->tags, strlen (data->tags)); 
    gtk_label_set_text (GTK_LABEL (priv->lbl_datemod_info), data->datemod);

    free_command_data (&data);

    return 0;
}

int
save_command (StarcmdCommandWindow **self, sqlite3 *db)
{
    StarcmdCommandWindowPrivate *priv;
    struct CommandData          *data;
    int                          err;

    // Get current date time
    char datemod[256] = {0};
    if ( (err = set_timestamp (datemod)) != 0)
        return 1;

    // Replace null fields with empty string
    priv = starcmd_command_window_get_instance_private (*self);
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

    // Save data to db
    data = malloc (sizeof (struct CommandData));
    data->name = strdup (priv->name);
    data->platform = strdup (priv->platform);
    data->os = strdup (priv->os);
    data->description = strdup (priv->description);
    data->commands = strdup (priv->commands);
    data->examples = strdup (priv->examples);
    data->references = strdup (priv->references);
    data->tags = strdup (priv->tags);
    data->datemod = strdup (datemod);

    if ( (err = starcmd_db_save (&db, (char *) priv->name_default, data)) != 0 )
    {
        starcmd_db_close (&db);
        return 1;
    }
    starcmd_db_close (&db);

    free_command_data (&data);

    return 0;
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdCommandWindow *
starcmd_command_window_new (void)
{
    return g_object_new (STARCMD_TYPE_COMMAND_WINDOW, NULL);
}

