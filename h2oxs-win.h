#ifndef __H2O_XSWIN_H
#define __H2O_XSWIN_H

#include <gtk/gtk.h>
#include "h2oxs.h"


#define H2O_XS_WINDOW_TYPE (h2o_xs_window_get_type ())
#define H2O_XS_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST \
                                      ((obj), H2O_XS_WINDOW_TYPE, \
                                       H2OxsWindow))


typedef struct _H2OxsWindow        H2OxsWindow;
typedef struct _H2OxsWindowClass   H2OxsWindowClass;


GType          h2o_xs_window_get_type (void);
H2OxsWindow   *h2o_xs_window_new      (H2OxsApp *app);


#endif /* __H2O_XSWIN_H */
