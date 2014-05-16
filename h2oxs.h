#ifndef __H2O_XSAPP_H
#define __H2O_XSAPP_H

#include <gtk/gtk.h>


#define H2O_XS_APP_TYPE (h2o_xs_app_get_type ())
#define H2O_XS_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                               H2O_XS_APP_TYPE, H2OxsApp))


typedef struct _H2OxsApp       H2OxsApp;
typedef struct _H2OxsAppClass  H2OxsAppClass;


GType           h2o_xs_app_get_type    (void);
H2OxsApp       *h2o_xs_app_new         (void);


#endif /* __H2O_XSAPP_H */
