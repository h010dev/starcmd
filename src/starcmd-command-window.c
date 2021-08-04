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

/* INTERNAL DECLARATIONS */

static void starcmd_command_window_dispose (GObject *object);
static void starcmd_command_window_finalize (GObject *object);

/* GOBJECT DEFINITION */

typedef struct _StarcmdCommandWindowPrivate StarcmdCommandWindowPrivate;

struct _StarcmdCommandWindowPrivate
{
    GtkWidget       *btn_cancel;
    GtkWidget       *btn_save;
    GtkWidget       *btn_browse;
    GtkWidget       *btn_download;
    const char      *name;
    const char      *platform;
    const char      *os;
    const char      *description;
    const char      *commands;
    const char      *examples;
    const char      *references;
    const char      *tags;
    const char      *datemod;
    const char      *icon;
} _StarcmdCommandWindowPrivate; /* get local access to instance attributes */

G_DEFINE_TYPE_WITH_PRIVATE (StarcmdCommandWindow, starcmd_command_window, GTK_TYPE_WINDOW);

/* CONSTANTS */

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-command-window.glade";

/* PROPERTIES */

enum {
    PROP_0,
    PROP_ID,
    LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

enum {
    SAVE_CMD,
    LAST_SIGNAL
};

static guint signals [LAST_SIGNAL];

/* METHOD DEFINITIONS */

static void
starcmd_command_window_get_property (GObject    *object,
                                     guint       prop_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    StarcmdCommandWindow *self = (StarcmdCommandWindow *) object;

    switch (prop_id)
    {
        case PROP_ID:
            g_value_set_int (value, starcmd_command_window_get_id (self));
            break;
    }
}

static void
starcmd_command_window_set_property (GObject          *object,
                                     guint             prop_id,
                                     const GValue     *value,
                                     GParamSpec       *pspec)
{
    StarcmdCommandWindow *self = (StarcmdCommandWindow *) object;

    switch (prop_id)
    {
        case PROP_ID:
            starcmd_command_window_set_id (self, (int) g_value_get_int (value));
            break;
    }
}

static void
starcmd_command_window_class_init (StarcmdCommandWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->get_property = starcmd_command_window_get_property;
    object_class->set_property = starcmd_command_window_set_property;
    object_class->dispose = starcmd_command_window_dispose;
    object_class->finalize = starcmd_command_window_finalize;

    properties [PROP_ID] =
        g_param_spec_int ("id",
                          "ID",
                          "The command's unique identifier",
                          -1, 10000, 0,
                          (G_PARAM_READWRITE));

    g_object_class_install_properties (object_class, LAST_PROP, properties);

    signals [SAVE_CMD] =
        g_signal_new ("save_cmd",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      0,
                      NULL, NULL, NULL,
                      G_TYPE_NONE, 0);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
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

/* CALLBACK HANDLERS */

void
on_btn_save_clicked (GtkButton *btn, gpointer user_data)
{

    sqlite3 *db;
    char    *err_msg = 0;
    int      err = 0;

    // Connect to database
    if (sqlite3_open ("starcmd-demo.db", &db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return;
    }

    err = save_command (db);
    sqlite3_close (db);
}

void
on_btn_cancel_clicked (GtkButton *btn, StarcmdCommandWindow *self)
{
    g_signal_emit (self, signals[SAVE_CMD], 0);
}

void
on_btn_browse_clicked (GtkButton *btn)
{
    printf ("browse btn clicked\n");
}

void
on_btn_download_clicked (GtkButton *btn)
{
    printf ("download btn clicked\n");
}

void
on_textentry_name_changed (GtkEditable *editable, gpointer user_data)
{
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (user_data);
    _StarcmdCommandWindowPrivate.name = gtk_entry_buffer_get_text (buffer);
}

void
on_textentry_platform_changed (GtkEditable *editable, gpointer user_data)
{
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (user_data);
    _StarcmdCommandWindowPrivate.platform = gtk_entry_buffer_get_text (buffer);
}

void
on_textentry_os_changed (GtkEditable *editable, gpointer user_data)
{
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (user_data);
    _StarcmdCommandWindowPrivate.os = gtk_entry_buffer_get_text (buffer);
}

void
on_buffer_description_changed (GtkTextBuffer *textbuffer, gpointer user_data)
{
    (void) user_data;
    GtkTextIter begin, end;
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    _StarcmdCommandWindowPrivate.description = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);
}

void
on_buffer_commands_changed (GtkTextBuffer *textbuffer, gpointer user_data)
{
    (void) user_data;
    GtkTextIter begin, end;
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    _StarcmdCommandWindowPrivate.commands = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);
}

void
on_buffer_examples_changed (GtkTextBuffer *textbuffer, gpointer user_data)
{
    (void) user_data;
    GtkTextIter begin, end;
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    _StarcmdCommandWindowPrivate.examples = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);
}

void
on_buffer_references_changed (GtkTextBuffer *textbuffer, gpointer user_data)
{
    (void) user_data;
    GtkTextIter begin, end;
    
    gtk_text_buffer_get_iter_at_offset (textbuffer, &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset (textbuffer, &end, (gint) -1);
    _StarcmdCommandWindowPrivate.references = gtk_text_buffer_get_text (textbuffer, &begin, &end, TRUE);
}

void
on_textentry_tags_changed (GtkEditable *editable, gpointer user_data)
{
    GtkEntryBuffer *buffer = GTK_ENTRY_BUFFER (user_data);
    _StarcmdCommandWindowPrivate.tags = gtk_entry_buffer_get_text (buffer);
}

/* HELPER METHODS */

int
load_command (void)
{
    return 0;
}

int
save_command (sqlite3 *db)
{
    int rc;
    sqlite3_stmt *res;

    char *sql = "INSERT INTO COMMANDS (name, platform, os, description, command, examples, refs, datemod, icon)"
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

    if ( (rc = sqlite3_prepare_v2 (db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (db));
        sqlite3_close (db);
        return 1;
    }

    char *datemod = "today";
    char *icon = "/img";

    sqlite3_bind_text (res, 1, _StarcmdCommandWindowPrivate.name, strlen (_StarcmdCommandWindowPrivate.name), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 2, _StarcmdCommandWindowPrivate.platform, strlen (_StarcmdCommandWindowPrivate.platform), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 3, _StarcmdCommandWindowPrivate.os, strlen (_StarcmdCommandWindowPrivate.os), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 4, _StarcmdCommandWindowPrivate.description, strlen (_StarcmdCommandWindowPrivate.description), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 5, _StarcmdCommandWindowPrivate.commands, strlen (_StarcmdCommandWindowPrivate.commands), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 6, _StarcmdCommandWindowPrivate.examples, strlen (_StarcmdCommandWindowPrivate.examples), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 7, _StarcmdCommandWindowPrivate.references, strlen (_StarcmdCommandWindowPrivate.references), SQLITE_TRANSIENT);
    //sqlite3_bind_text (res, 9, tags, strlen (tags), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 8, datemod, strlen (datemod), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 9, icon, strlen (icon), SQLITE_TRANSIENT);
    sqlite3_step (res);
    sqlite3_finalize (res);

    return 0;
}

int
edit_command (void)
{
    return 0;
}

int
delete_command (void)
{
    return 0;
}

/* PUBLIC METHOD DEFINITIONS */

StarcmdCommandWindow *
starcmd_command_window_new (void)
{
    return g_object_new (STARCMD_TYPE_COMMAND_WINDOW, NULL);
}

/* GETTERS */

int
starcmd_command_window_get_id (StarcmdCommandWindow *self)
{
    return 0;
    //return self->id;
}

/* SETTERS */

void
starcmd_command_window_set_id (StarcmdCommandWindow *self,
                               int                   id)
{
    
   // self->id = id; 
}
