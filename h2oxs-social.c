#include "h2oxs-social.h"
#include "h2oxs-oauth.h"
#include <gio/gio.h>

static H2OxsOauth *assistant_oauth = NULL;
static gchar      *assistant_code = NULL;


const char *config_path ()
{
    const char *path = g_get_user_config_dir ();
    GFile *confd = g_file_new_for_path (path);
    GFile *conff = g_file_get_child (confd, "h2o.conf");

    const char *cpath = g_file_get_path (conff);

    g_object_unref (confd);
    g_object_unref (conff);
    return cpath;
}

GKeyFile *get_config () {
    GKeyFile *conf = g_key_file_new ();

    g_key_file_load_from_file (conf, config_path(), G_KEY_FILE_NONE, NULL);

    return conf;
}

void
on_assistant_close_cancel (GtkWidget *w, gpointer data)
{
    gtk_widget_destroy (w);
}

void
on_entry_changed (GtkWidget *w, gpointer data)
{
    GtkAssistant *assistant = GTK_ASSISTANT (data);
    GtkWidget *current_page;
    gint page_number;

    page_number = gtk_assistant_get_current_page (assistant);
    current_page = gtk_assistant_get_nth_page (assistant, page_number);
    if (assistant_code) {
        g_free (assistant_code);
    }
    assistant_code = g_strdup (gtk_entry_get_text (GTK_ENTRY (w)));

    if (assistant_code && *assistant_code)
        gtk_assistant_set_page_complete (assistant, current_page, TRUE);
    else
        gtk_assistant_set_page_complete (assistant, current_page, FALSE);
}

static void
on_assistant_apply (GtkWidget *widget, gpointer data)
{
    GtkAssistant *assistant = GTK_ASSISTANT (data);
    int success = FALSE;
    success = h2o_xs_social_twitter_confirm (assistant_oauth, assistant_code);

    if (success) {
        g_object_unref (assistant_oauth);
        g_free (assistant_code);
        assistant_oauth = NULL;
        assistant_code = NULL;
        gtk_window_close (GTK_WINDOW (assistant));
    } else {
        GtkWidget *d = gtk_message_dialog_new (GTK_WINDOW (assistant),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Invalid code, try again");
        gtk_window_set_title (GTK_WINDOW (d), "Error");
        gtk_dialog_run (GTK_DIALOG (d));
        gtk_widget_destroy (d);
    }
}

void
assistant_page1 (GtkWidget *assistant) {
    GtkWidget *box, *label, *button;

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width (GTK_CONTAINER (box), 12);

    label = gtk_label_new ("You need to authorize Agüita to work with your twitter account. "
                           "Click in the button below, copy the code and continue with the proccess");
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);


    gchar *url = h2o_xs_social_twitter_new (assistant_oauth);

    button = gtk_link_button_new_with_label (url,"Go to confirm");
    gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);

    gtk_widget_show_all (box);
    gtk_assistant_append_page (GTK_ASSISTANT (assistant), box);
    gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box, "Getting twitter confirmation code");
    gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box, GTK_ASSISTANT_PAGE_INTRO);
    gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), box, TRUE);
}

void
assistant_page2 (GtkWidget *assistant) {
    GtkWidget *box, *label, *entry;

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_container_set_border_width (GTK_CONTAINER (box), 12);

    label = gtk_label_new ("Code: ");
    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);

    entry = gtk_entry_new ();
    gtk_entry_set_activates_default (GTK_ENTRY (entry), TRUE);
    gtk_box_pack_start (GTK_BOX (box), entry, TRUE, TRUE, 0);

    g_signal_connect (G_OBJECT (entry), "changed",
                      G_CALLBACK (on_entry_changed), assistant);

    gtk_widget_show_all (box);
    gtk_assistant_append_page (GTK_ASSISTANT (assistant), box);
    gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box, "Confirm your twitter code");
    gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box, GTK_ASSISTANT_PAGE_CONFIRM);
    gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), box, FALSE);
}

void
h2o_xs_social_twitter_account_popup (GtkWidget *widget, H2OxsWindow *win) {
    GtkWidget *assistant = gtk_assistant_new ();
    gtk_window_set_title (GTK_WINDOW (assistant), "New twitter account");
    g_signal_connect (G_OBJECT (assistant), "cancel",
                      G_CALLBACK (on_assistant_close_cancel), NULL);
    g_signal_connect (G_OBJECT (assistant), "apply",
            G_CALLBACK (on_assistant_apply), assistant);

    gtk_window_set_position (GTK_WINDOW (assistant), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_modal (GTK_WINDOW (assistant), TRUE);
    gtk_window_set_transient_for (GTK_WINDOW (assistant), GTK_WINDOW (win));
    gtk_window_set_default_size (GTK_WINDOW (assistant), 600, 300);

    if (assistant_oauth) {
        g_object_unref (assistant_oauth);
    }
    assistant_oauth = h2o_xs_oauth_new ();

    assistant_page1 (assistant);
    assistant_page2 (assistant);
    gtk_widget_show (assistant);


}

// Return a list of H2OxsOauth with twitter registered accounts
GList* h2o_xs_social_twitter_accounts () {
    GList *accounts = NULL;
    GError *error = NULL;

    GKeyFile *conf = get_config ();

    gsize size = 0;
    int i = 0;
    gchar **groups = g_key_file_get_groups (conf, &size);
    for (i=0; i<size; i++) {
        gchar *group = groups[i];
        gchar *tk, *tks, *sn;

        tk = g_key_file_get_string(conf, group, "token", &error);
        tks = g_key_file_get_string(conf, group, "token_secret", &error);
        sn = g_key_file_get_string(conf, group, "screen_name", &error);

        H2OxsOauth *auth = h2o_xs_oauth_new ();

        h2o_xs_oauth_set_token (auth, tk);
        h2o_xs_oauth_set_token_secret (auth, tks);
        h2o_xs_oauth_set_screen_name (auth, sn);

        accounts = g_list_prepend (accounts, auth);
    }

    g_key_file_free (conf);

    return accounts;
}

gchar*
h2o_xs_social_twitter_new (H2OxsOauth *oauth) {
    gchar *ret = NULL;

    h2o_xs_oauth_request_token (oauth, "https://api.twitter.com/");
    ret = g_strdup_printf ("https://api.twitter.com/oauth/authorize?oauth_token=%s",
              h2o_xs_oauth_get_token(oauth));

    return ret;
}

int h2o_xs_social_twitter_confirm (H2OxsOauth *oauth, gchar *code)
{
    int success = FALSE;
    success = h2o_xs_oauth_verify (oauth, "https://api.twitter.com/", code);
    if (success) {
        h2o_xs_social_twitter_store (oauth);
        return TRUE;
    } else {
        return FALSE;
    }
}

void h2o_xs_social_twitter_store (H2OxsOauth *oauth)
{
    GError *error = NULL;
    GKeyFile *conf = get_config ();
    const char *cpath = config_path();
    char *name = h2o_xs_oauth_get_screen_name (oauth);
    char *tk = h2o_xs_oauth_get_token (oauth);
    char *tks = h2o_xs_oauth_get_token_secret (oauth);

    g_key_file_set_value (conf, name, "screen_name", name);
    g_key_file_set_value (conf, name, "token", tk);
    g_key_file_set_value (conf, name, "token_secret", tks);

    g_key_file_save_to_file (conf, cpath, &error);
    if (error != NULL) {
        g_printf ("Unable to store credentials: %s\n", error->message);
        g_error_free (error);
    }

    g_key_file_free (conf);
}

//void h2o_xs_social_twitter_home ();
