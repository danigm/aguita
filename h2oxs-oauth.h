#ifndef __H2O_XS_OAUTH_H
#define __H2O_XS_OAUTH_H

#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <json-glib/json-glib.h>

#define H2O_XS_OAUTH_TYPE (h2o_xs_oauth_get_type ())
#define H2O_XS_OAUTH(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                               H2O_XS_OAUTH_TYPE, H2OxsOauth))


typedef struct _H2OxsOauth       H2OxsOauth;
typedef struct _H2OxsOauthClass  H2OxsOauthClass;

GType           h2o_xs_oauth_get_type    (void);
H2OxsOauth     *h2o_xs_oauth_new         (void);


gboolean        h2o_xs_oauth_request_token (H2OxsOauth *oauth,
                                            char *api_url);
gboolean        h2o_xs_oauth_verify        (H2OxsOauth *oauth,
                                            char *api_url,
                                            char *code);

char*           h2o_xs_oauth_get_token        (H2OxsOauth *oauth);
char*           h2o_xs_oauth_get_token_secret (H2OxsOauth *oauth);

char*           h2o_xs_oauth_get_screen_name  (H2OxsOauth *oauth);

void            h2o_xs_oauth_set_api_key      (H2OxsOauth *oauth,
                                               char *apikey);
void            h2o_xs_oauth_set_api_secret   (H2OxsOauth *oauth,
                                               char *secret);

void            h2o_xs_oauth_set_token        (H2OxsOauth *oauth,
                                               char *token);
void            h2o_xs_oauth_set_token_secret (H2OxsOauth *oauth,
                                               char *token_secret);
void            h2o_xs_oauth_set_screen_name  (H2OxsOauth *oauth,
                                               char *screen_name);

G_GNUC_NULL_TERMINATED
JsonParser     *h2o_xs_oauth_query            (H2OxsOauth *oauth,
                                               char *method,
                                               char *api_url,
                                               char *resource,
                                               ...);

#endif // __H2O_XS_OAUTH_H
