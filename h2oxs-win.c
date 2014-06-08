#include "h2oxs.h"
#include "h2oxs-win.h"
#include "h2oxs-social.h"
#include <gtk/gtk.h>

void show_no_account (H2OxsWindow *w);
void show_timeline (H2OxsWindow *w, H2OxsOauth *oauth);
void load_accounts (H2OxsWindow *w);


struct _H2OxsWindow
{
    GtkApplicationWindow parent;

    GtkWidget *header;
    GtkWidget *body;
    GtkWidget *search_button;
    GtkWidget *timeline;

    GtkWidget *no_account;

    // The list of H2OxsOauth
    GList *accounts;
    Timeline social_timeline;
};


struct _H2OxsWindowClass
{
    GtkApplicationWindowClass parent_class;
};

G_DEFINE_TYPE(H2OxsWindow, h2o_xs_window,
              GTK_TYPE_APPLICATION_WINDOW);


static void
h2o_xs_window_dispose (GObject *object)
{
    H2OxsWindow *win = H2O_XS_WINDOW (object);
    G_OBJECT_CLASS (h2o_xs_window_parent_class)->dispose (object);
}

static void
h2o_xs_window_init (H2OxsWindow *app)
{
}

static void
h2o_xs_window_class_init (H2OxsWindowClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);
    object_class->dispose = h2o_xs_window_dispose;
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
                          gtk_image_new_from_icon_name ("text-editor-symbolic", 1));
    gtk_header_bar_pack_start (GTK_HEADER_BAR (w->header),
                               w->search_button);
    gtk_widget_show_all (w->header);

    w->body = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (w), w->body);
    gtk_widget_show_all (w->body);

    w->social_timeline.w = NULL;
    w->social_timeline.o = NULL;
    w->social_timeline.since_id[0] = 0;

    w->accounts = NULL;
    load_accounts (w);

    return w;
}

void
load_accounts (H2OxsWindow *w) {
    if (!w->accounts || g_list_length (w->accounts) == 0) {
        g_list_free_full (w->accounts, g_object_unref);
        w->accounts = NULL;
    }

    w->accounts = h2o_xs_social_twitter_accounts ();

    if (!w->accounts || g_list_length (w->accounts) == 0) {
        show_no_account (w);
    } else {
        show_timeline (w, (H2OxsOauth*) g_list_nth_data (w->accounts, 0));
    }
}

void
show_no_account (H2OxsWindow *w) {
    GtkWidget *label = gtk_label_new ("There's no account");
    GtkWidget *button = gtk_button_new_with_label ("Add an account");

    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (h2o_xs_social_twitter_account_popup), w);

    gtk_label_set_markup (GTK_LABEL (label), "<span font_size=\"xx-large\">There's no account</span>");

    w->no_account = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width (GTK_CONTAINER (w->no_account), 10);

    gtk_container_add (GTK_CONTAINER (w->no_account), label);
    gtk_container_add (GTK_CONTAINER (w->no_account), button);

    gtk_container_add (GTK_CONTAINER (w->body), w->no_account);
    gtk_widget_show_all (w->no_account);
}


gboolean
reload (gpointer user_data)
{
    H2OxsWindow *w = H2O_XS_WINDOW (user_data);
    h2o_xs_social_twitter_home (w->social_timeline.o, w->timeline, &(w->social_timeline));
    g_printf ("RELOADING\n");
}

void
show_timeline (H2OxsWindow *w, H2OxsOauth *oauth)
{
    w->timeline = gtk_list_box_new ();

    w->social_timeline.w = w;
    w->social_timeline.o = oauth;

    GtkWidget *scrolled = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled), w->timeline);
    gtk_box_pack_start (GTK_BOX (w->body), scrolled, TRUE, TRUE, 0);
    g_timeout_add (60000, reload, w);

    h2o_xs_social_twitter_home (oauth, w->timeline, &(w->social_timeline));
    gtk_widget_show_all (w->body);
}
