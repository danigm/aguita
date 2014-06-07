#include "h2oxs-message.h"
#include <gio/gio.h>


void _load_image (GtkImage *img, gchar *url);



G_DEFINE_TYPE(H2OxsMessage, h2o_xs_message, G_TYPE_OBJECT);
G_DEFINE_TYPE(H2OxsMessageRow, h2o_xs_message_row, GTK_TYPE_LIST_BOX_ROW);

static void
h2o_xs_message_init (H2OxsMessage *msg)
{
}

static void
h2o_xs_message_class_init (H2OxsMessageClass *class)
{
}

static void
h2o_xs_message_row_init (H2OxsMessageRow *msg)
{
}

static void
h2o_xs_message_row_class_init (H2OxsMessageRowClass *class)
{
}


H2OxsMessage *
h2o_xs_message_new (void)
{
  return g_object_new (H2O_XS_MESSAGE_TYPE, NULL);
}


H2OxsMessageRow *
h2o_xs_message_row_new (H2OxsMessage *msg)
{
    char *s = NULL;
    H2OxsMessageRow *row;
    GtkWidget *hbox = NULL;

    row = g_object_new (H2O_XS_MESSAGE_ROW_TYPE, NULL);
    row->message = msg;

    row->body = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (row), row->body);

    row->source_name = (GtkLabel*)gtk_label_new (row->message->sender_name);
    row->source_nick = (GtkLabel*)gtk_label_new (row->message->sender_nick);
    row->content_label = (GtkLabel*)gtk_label_new (row->message->message);

    s = g_strdup_printf ("<b>%s</b>", row->message->sender_name);
    gtk_label_set_markup (row->source_name, s);
    g_free (s);

    s = g_strdup_printf ("<span color=\"#aaa\">@%s</span>", row->message->sender_nick);
    gtk_label_set_markup (row->source_nick, s);
    g_free (s);

    row->avatar = (GtkImage*)gtk_image_new ();
    _load_image(row->avatar, row->message->avatar);

    gtk_label_set_line_wrap (GTK_LABEL (row->content_label), TRUE);

    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (hbox), GTK_WIDGET (row->source_name), FALSE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX (hbox), GTK_WIDGET (row->source_nick), FALSE, FALSE, 10);
    gtk_widget_set_valign (GTK_WIDGET (row->avatar), GTK_ALIGN_START);
    gtk_widget_set_halign (GTK_WIDGET (row->source_nick), GTK_ALIGN_START);
    gtk_widget_set_halign (GTK_WIDGET (row->content_label), GTK_ALIGN_START);

    gtk_grid_attach (GTK_GRID (row->body), GTK_WIDGET(row->avatar), 0, 0, 1, 2);
    gtk_grid_attach (GTK_GRID (row->body), GTK_WIDGET(hbox), 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (row->body), GTK_WIDGET(row->content_label), 1, 1, 11, 1);
    gtk_grid_attach (GTK_GRID (row->body), gtk_separator_new (GTK_ORIENTATION_HORIZONTAL), 0, 2, 12, 1);

    return row;
}



void _cb (GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    GFile *f = G_FILE (source_object);
    gchar *contents = NULL;
    gsize length = 0;
    GError *error = NULL;
    GtkImage *img = GTK_IMAGE (user_data);
    GdkPixbufLoader *pixbuf_loader = NULL;
    GdkPixbuf *pixbuf = NULL;

    g_file_load_contents_finish (f, res, &contents, &length, NULL, &error);

    pixbuf_loader = gdk_pixbuf_loader_new ();

    if (gdk_pixbuf_loader_write (pixbuf_loader,
                (const guchar *)contents,
                length,
                &error))
    {
        gdk_pixbuf_loader_close (pixbuf_loader, NULL);
        pixbuf = gdk_pixbuf_loader_get_pixbuf (pixbuf_loader);
        gtk_image_set_from_pixbuf (img, pixbuf);
    }
}

void _load_image (GtkImage *img, gchar *url)
{
    GFile *f = g_file_new_for_uri (url);
    g_file_load_contents_async (f, NULL, _cb, img);
}
