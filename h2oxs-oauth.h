#ifndef __H2O_XS_OAUTH_H
#define __H2O_XS_OAUTH_H

#include <glib-object.h>
#include <stdlib.h>
#include <oauth.h>

#define H2O_XS_OAUTH_TYPE (h2o_xs_oauth_get_type ())
#define H2O_XS_OAUTH(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                               H2O_XS_OAUTH_TYPE, H2OxsOauth))


typedef struct _H2OxsOauth       H2OxsOauth;
typedef struct _H2OxsOauthClass  H2OxsOauthClass;

GType           h2o_xs_oauth_get_type    (void);
H2OxsOauth     *h2o_xs_oauth_new         (void);


int             h2o_xs_oauth_request_token (H2OxsOauth *oauth,
                                            char *request_token_uri);
int             h2o_xs_oauth_verify        (H2OxsOauth *oauth,
                                            char *request_token_uri,
                                            char *code);

char*           h2o_xs_oauth_get_token        (H2OxsOauth *oauth);
char*           h2o_xs_oauth_get_token_secret (H2OxsOauth *oauth);

char*           h2o_xs_oauth_get_user_id      (H2OxsOauth *oauth);
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
                                               char *screen_name, char *user_id);

char*           h2o_xs_oauth_get              (H2OxsOauth *oauth,
                                               char *url);

#endif // __H2O_XS_OAUTH_H
