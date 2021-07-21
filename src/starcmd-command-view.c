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
};

G_DEFINE_TYPE (StarcmdCommandView, starcmd_command_view, GTK_TYPE_GRID);

/* PROPERTIES */

enum {
    PROP_0,
    PROP_NAME,
    LAST_PROP
};

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
    g_object_class_install_properties (object_class, LAST_PROP, properties);

    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass), 
                                                 "/org/h010dev/starcmd/starcmd-command-view.glade");
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

const gchar *
starcmd_command_view_get_name (StarcmdCommandView *self)
{
    return self->name;
}

void
starcmd_command_view_set_name (StarcmdCommandView *self,
                               const gchar        *name)
{
    if (g_strcmp0 (name, self->name) == 0) {
        g_free (self->name);
        self->name = g_strdup (name);
    }
}
