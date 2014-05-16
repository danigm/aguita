#include "h2oxs.h"
#include "h2oxs-win.h"
#include <gtk/gtk.h>

void show_no_account (H2OxsWindow *w);



struct _H2OxsWindow
{
    GtkApplicationWindow parent;

    GtkWidget *header;
    GtkWidget *body;
    GtkWidget *search_button;

    GtkWidget *no_account;
};


struct _H2OxsWindowClass
{
    GtkApplicationWindowClass parent_class;
};

G_DEFINE_TYPE(H2OxsWindow, h2o_xs_window,
              GTK_TYPE_APPLICATION_WINDOW);

static void
h2o_xs_window_init (H2OxsWindow *app)
{
}

static void
h2o_xs_window_class_init (H2OxsWindowClass *class)
{
}

H2OxsWindow *
h2o_xs_window_new (H2OxsApp *app)
{
    H2OxsWindow * w = g_object_new (H2O_XS_WINDOW_TYPE,
                                          "application", app, NULL);
    gtk_window_set_default_size (GTK_WINDOW (w), 400, 600);
    gtk_window_set_title (GTK_WINDOW (w), "Agüita");

    w->header = gtk_header_bar_new ();
    gtk_header_bar_set_title (GTK_HEADER_BAR (w->header), "Agüita");
    gtk_window_set_titlebar (GTK_WINDOW (w), w->header);
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (w->header), TRUE);

    w->search_button = gtk_menu_button_new ();
    gtk_button_set_image (GTK_BUTTON (w->search_button),
                          gtk_image_new_from_icon_name ("edit-find-symbolic", 1));
    gtk_header_bar_pack_start (GTK_HEADER_BAR (w->header),
                               w->search_button);
    gtk_widget_show_all (w->header);

    w->body = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (w), w->body);
    gtk_widget_show_all (w->body);

    show_no_account (H2O_XS_WINDOW (w));

    return w;
}

void
show_no_account (H2OxsWindow *w) {
    GtkWidget *label = gtk_label_new ("There's no account");
    GtkWidget *button = gtk_button_new_with_label ("Add an account");

    gtk_label_set_markup (GTK_LABEL (label), "<span font_size=\"xx-large\">There's no account</span>");

    w->no_account = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width (GTK_CONTAINER (w->no_account), 10);

    gtk_container_add (GTK_CONTAINER (w->no_account), label);
    gtk_container_add (GTK_CONTAINER (w->no_account), button);

    gtk_container_add (GTK_CONTAINER (w->body), w->no_account);
    gtk_widget_show_all (w->no_account);
}
