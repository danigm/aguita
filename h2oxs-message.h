#ifndef __H2O_XSMSG_H
#define __H2O_XSMSG_H


#include <gtk/gtk.h>


#define H2O_XS_MESSAGE_TYPE (h2o_xs_message_get_type ())
#define H2O_XS_MESSAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                               H2O_XS_MESSAGE_TYPE, H2OxsMessage))

#define H2O_XS_MESSAGE_ROW_TYPE (h2o_xs_message_row_get_type ())
#define H2O_XS_MESSAGE_ROW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                               H2O_XS_MESSAGE_ROW_TYPE, H2OxsMessageRow))

typedef struct _H2OxsMessage       H2OxsMessage;
typedef struct _H2OxsMessageClass  H2OxsMessageClass;

typedef struct _H2OxsMessageRow       H2OxsMessageRow;
typedef struct _H2OxsMessageRowClass  H2OxsMessageRowClass;

struct _H2OxsMessage
{
  GObject parent;

  char *avatar;
  char *sender_nick;
  char *sender_name;
  char *message;
};

struct _H2OxsMessageClass
{
  GObjectClass parent_class;
};

struct _H2OxsMessageRow
{
  GtkListBoxRow parent;

  GtkWidget *body;
  H2OxsMessage *message;
  GtkImage *avatar;
  GtkLabel *content_label;
  GtkLabel *source_nick;
  GtkLabel *source_name;
};

struct _H2OxsMessageRowClass
{
  GtkListBoxRowClass parent_class;
};

GType             h2o_xs_message_get_type    (void);
H2OxsMessage     *h2o_xs_message_new         (void);

GType             h2o_xs_message_row_get_type    (void);
H2OxsMessageRow  *h2o_xs_message_row_new         (H2OxsMessage *msg);

#endif // __H2O_XSMSG_H
