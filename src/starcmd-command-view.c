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

/* 
 * References:
 *     GObject: https://www.youtube.com/watch?v=1-_EBEr0fxI&t=0s
 *     GObject: https://www.youtube.com/watch?v=WZvX3WLS7Hw&t=3s
 *     Example: https://github.com/elboulangero/goodvibes/blob/master/src/ui/gv-playlist-view.c
 */
 
#include <gtk/gtk.h>

#include "starcmd-command-view.h"

/* INTERNAL DECLARATIONS */

static void starcmd_command_view_dispose (GObject *object);
static void starcmd_command_view_finalize (GObject *object);

/* GOBJECT DEFINITION */

struct _StarcmdCommandView
{
    GtkApplicationWindow  parent;
    gchar                *name;
    gchar                *platform;
    gchar                *os;
    gchar                *desc;
    gchar                *cmds;
    gchar                *exs;
    gchar                *refs;
    gchar                *datemod;
    gchar                *icon;
    gboolean              fav;
    GtkWidget            *lbl_cmd_name_info;
    GtkWidget            *lbl_cmd_platform_info;
    GtkWidget            *lbl_cmd_os_info;
    GtkWidget            *lbl_cmd_desc_info;
    GtkWidget            *lbl_cmd_cmds_info;
    GtkWidget            *lbl_cmd_exs_info;
    GtkWidget            *lbl_cmd_refs_info;
    GtkWidget            *lbl_cmd_datemod_info;
    GtkWidget            *img_cmd_icon;
    GtkWidget            *btn_fav;
    GtkWidget            *btn_edit;
};

G_DEFINE_TYPE (StarcmdCommandView, starcmd_command_view, GTK_TYPE_GRID);

/* PROPERTIES */

enum {
    PROP_0,
    PROP_NAME,
    PROP_PLATFORM,
    PROP_OS,
    PROP_DESC,
    PROP_CMDS,
    PROP_EXS,
    PROP_REFS,
    PROP_DATEMOD,
    PROP_ICON,
    PROP_FAV,
    LAST_PROP
};

static const gchar *RESOURCE_PATH = "/org/h010dev/starcmd/starcmd-command-view.glade";

static GParamSpec *properties [LAST_PROP];

/* METHOD DEFINITIONS */

static void
starcmd_command_view_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    StarcmdCommandView *self = (StarcmdCommandView *) object;

    switch (prop_id) {
        case PROP_NAME:
            g_value_set_string (value, starcmd_command_view_get_name (self));
            break;
        case PROP_PLATFORM:
            g_value_set_string (value, starcmd_command_view_get_platform (self));
            break;
        case PROP_OS:
            g_value_set_string (value, starcmd_command_view_get_os (self));
            break;
        case PROP_DESC:
            g_value_set_string (value, starcmd_command_view_get_desc (self));
            break;
        case PROP_CMDS:
            g_value_set_string (value, starcmd_command_view_get_cmds (self));
            break;
        case PROP_EXS:
            g_value_set_string (value, starcmd_command_view_get_exs (self));
            break;
        case PROP_REFS:
            g_value_set_string (value, starcmd_command_view_get_refs (self));
            break;
        case PROP_DATEMOD:
            g_value_set_string (value, starcmd_command_view_get_datemod (self));
            break;
        case PROP_ICON:
            g_value_set_string (value, starcmd_command_view_get_icon (self));
            break;
        case PROP_FAV:
            g_value_set_boolean (value, starcmd_command_view_get_fav (self));
            break;
    }
}

static void
starcmd_command_view_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
    StarcmdCommandView *self = (StarcmdCommandView *) object;

    switch (prop_id) {
        case PROP_NAME:
            starcmd_command_view_set_name (self, g_value_get_string (value));
            break;
        case PROP_PLATFORM:
            starcmd_command_view_set_platform (self, g_value_get_string (value));
            break;
        case PROP_OS:
            starcmd_command_view_set_os (self, g_value_get_string (value));
            break;
        case PROP_DESC:
            starcmd_command_view_set_desc (self, g_value_get_string (value));
            break;
        case PROP_CMDS:
            starcmd_command_view_set_cmds (self, g_value_get_string (value));
            break;
        case PROP_EXS:
            starcmd_command_view_set_exs (self, g_value_get_string (value));
            break;
        case PROP_REFS:
            starcmd_command_view_set_refs (self, g_value_get_string (value));
            break;
        case PROP_DATEMOD:
            starcmd_command_view_set_datemod (self, g_value_get_string (value));
            break;
        case PROP_ICON:
            starcmd_command_view_set_icon (self, g_value_get_string (value));
            break;
        case PROP_FAV:
            starcmd_command_view_set_fav (self, g_value_get_boolean (value));
            break;
    }
}

static void
starcmd_command_view_class_init (StarcmdCommandViewClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->get_property = starcmd_command_view_get_property;
    object_class->set_property = starcmd_command_view_set_property;
    object_class->dispose = starcmd_command_view_dispose;
    object_class->finalize = starcmd_command_view_finalize;

    properties [PROP_NAME] =
        g_param_spec_string ("name",
                             "Name",
                             "The name of the command",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_PLATFORM] =
        g_param_spec_string ("platform",
                             "Platform",
                             "The platform the command is running on",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_OS] =
        g_param_spec_string ("os",
                             "OS",
                             "The operating system the command is running on",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_DESC] =
        g_param_spec_string ("description",
                             "Description",
                             "The command description",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_CMDS] =
        g_param_spec_string ("commands",
                             "Commands",
                             "The command(s) to run",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_EXS] =
        g_param_spec_string ("examples",
                             "Examples",
                             "Implementation examples using this command",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_REFS] =
        g_param_spec_string ("references",
                             "References",
                             "Any references for this command",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_DATEMOD] =
        g_param_spec_string ("datemod", /* NOTE: Can't have underscores in canonical name param */
                             "Datemod",
                             "Date command was last modified",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_ICON] =
        g_param_spec_string ("icon",
                             "Icon",
                             "The command icon",
                             NULL,
                             (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    properties [PROP_FAV] =
        g_param_spec_boolean ("favorite",
                             "Favorite",
                             "Whether or not command is favorited by user",
                             FALSE,
                             (G_PARAM_READWRITE));

    g_object_class_install_properties (object_class, LAST_PROP, properties);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), RESOURCE_PATH);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_name_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_platform_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_os_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_desc_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_cmds_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_exs_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_refs_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, lbl_cmd_datemod_info);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, img_cmd_icon);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, btn_fav);
    gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass), StarcmdCommandView, btn_edit);
}

static void
starcmd_command_view_init (StarcmdCommandView *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}

static void
starcmd_command_view_dispose (GObject *object)
{
    StarcmdCommandView *self = STARCMD_COMMAND_VIEW (object); 

    G_OBJECT_CLASS (starcmd_command_view_parent_class)->dispose (object);
}

static void
starcmd_command_view_finalize (GObject *gobject)
{
    G_OBJECT_CLASS (starcmd_command_view_parent_class)->finalize (gobject);
}

/* PUBLIC METHOD DEFINITIONS */

GtkWidget *
starcmd_command_view_new (void)
{
    return g_object_new (STARCMD_TYPE_COMMAND_VIEW, NULL);
}

/* GETTERS */

const gchar *
starcmd_command_view_get_name (StarcmdCommandView *self)
{
    return self->name;
}

const gchar *
starcmd_command_view_get_platform (StarcmdCommandView *self)
{
    return self->platform;
}

const gchar *
starcmd_command_view_get_os (StarcmdCommandView *self)
{
    return self->os;
}

const gchar *
starcmd_command_view_get_desc (StarcmdCommandView *self)
{
    return self->desc;
}

const gchar *
starcmd_command_view_get_cmds (StarcmdCommandView *self)
{
    return self->cmds;
}

const gchar *
starcmd_command_view_get_exs (StarcmdCommandView *self)
{
    return self->exs;
}

const gchar *
starcmd_command_view_get_refs (StarcmdCommandView *self)
{
    return self->refs;
}

const gchar *
starcmd_command_view_get_datemod (StarcmdCommandView *self)
{
    return self->datemod;
}

const gchar *
starcmd_command_view_get_icon (StarcmdCommandView *self)
{
    return self->icon;
}

const gboolean
starcmd_command_view_get_fav (StarcmdCommandView *self)
{
    return self->fav;
}

/* SETTERS */

void
starcmd_command_view_set_name (StarcmdCommandView *self,
                               const gchar        *name)
{
    if (g_strcmp0 (name, self->name) == 0) {
        g_free (self->name);
        self->name = g_strdup (name);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_name_info), name);
}

void
starcmd_command_view_set_platform (StarcmdCommandView *self,
                                   const gchar        *platform)
{
    if (g_strcmp0 (platform, self->platform) == 0) {
        g_free (self->platform);
        self->platform = g_strdup (platform);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_platform_info), platform);
}

void
starcmd_command_view_set_os (StarcmdCommandView *self,
                             const gchar        *os)
{
    if (g_strcmp0 (os, self->os) == 0) {
        g_free (self->os);
        self->os = g_strdup (os);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_os_info), os);
}

void
starcmd_command_view_set_desc (StarcmdCommandView *self,
                               const gchar        *desc)
{
    if (g_strcmp0 (desc, self->desc) == 0) {
        g_free (self->desc);
        self->desc = g_strdup (desc);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_desc_info), desc);
}

void
starcmd_command_view_set_cmds (StarcmdCommandView *self,
                               const gchar        *cmds)
{
    if (g_strcmp0 (cmds, self->cmds) == 0) {
        g_free (self->cmds);
        self->cmds = g_strdup (cmds);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_cmds_info), cmds);
}

void
starcmd_command_view_set_exs (StarcmdCommandView *self,
                              const gchar        *exs)
{
    if (g_strcmp0 (exs, self->exs) == 0) {
        g_free (self->exs);
        self->exs = g_strdup (exs);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_exs_info), exs);
}

void
starcmd_command_view_set_refs (StarcmdCommandView *self,
                               const gchar        *refs)
{
    if (g_strcmp0 (refs, self->refs) == 0) {
        g_free (self->refs);
        self->refs = g_strdup (refs);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_refs_info), refs);
}

void
starcmd_command_view_set_datemod (StarcmdCommandView *self,
                                  const gchar        *datemod)
{
    if (g_strcmp0 (datemod, self->datemod) == 0) {
        g_free (self->datemod);
        self->datemod = g_strdup (datemod);
    }

    gtk_label_set_text (GTK_LABEL (self->lbl_cmd_datemod_info), datemod);
}

void
starcmd_command_view_set_icon (StarcmdCommandView *self,
                               const gchar        *icon)
{
    if (g_strcmp0 (icon, self->icon) == 0) {
        g_free (self->icon);
        self->icon = g_strdup (icon);
    }

    // Assign icon by path
}

void
starcmd_command_view_set_fav (StarcmdCommandView *self,
                              const gboolean      fav)
{
    self->fav = fav;

    // Highlight/un-highlight fav icon
}