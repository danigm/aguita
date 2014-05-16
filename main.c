#include <gtk/gtk.h>

#include "h2oxs.h"


int main (int argc, char *argv[])
{
    return g_application_run (G_APPLICATION (h2o_xs_app_new ()),
                                             argc, argv);
}
