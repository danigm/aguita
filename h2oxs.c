#include <gtk/gtk.h>

#include "h2oxs.h"
#include "h2oxs-win.h"


struct _H2OxsApp
{
    GtkApplication parent;
};


struct _H2OxsAppClass
{
    GtkApplicationClass parent_class;
};


G_DEFINE_TYPE(H2OxsApp, h2o_xs_app, GTK_TYPE_APPLICATION);


static void
h2o_xs_app_init (H2OxsApp *app)
{
}

static void
h2o_xs_app_activate (GApplication *app)
{
    H2OxsWindow *win;

    win = h2o_xs_window_new (H2O_XS_APP (app));
    gtk_window_present (GTK_WINDOW (win));
}

static void
h2o_xs_app_class_init (H2OxsAppClass *class)
{
    G_APPLICATION_CLASS (class)->activate = h2o_xs_app_activate;
}

H2OxsApp *
h2o_xs_app_new (void)
{
  return g_object_new (H2O_XS_APP_TYPE,
                       "application-id", "com.wadobo.h2oxs",
                       NULL);
}
