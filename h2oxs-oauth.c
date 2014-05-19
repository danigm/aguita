#include "h2oxs-oauth.h"

struct _H2OxsOauth
{
    GObject parent;

    char *api_key;
    char *api_secret;
    char *token;
    char *token_secret;

    char *user_id;
    char *screen_name;
};


struct _H2OxsOauthClass
{
    GObjectClass parent_class;
};


G_DEFINE_TYPE(H2OxsOauth, h2o_xs_oauth, G_TYPE_OBJECT);


static void
h2o_xs_oauth_dispose (GObject *object)
{
    H2OxsOauth *oauth = H2O_XS_OAUTH (object);
    if (oauth->api_key)
        g_free (oauth->api_key);
    if (oauth->api_secret)
        g_free (oauth->api_secret);
    if (oauth->user_id)
        g_free (oauth->user_id);
    if (oauth->screen_name)
        g_free (oauth->screen_name);

    G_OBJECT_CLASS (h2o_xs_oauth_parent_class)->dispose (object);
}


static void
h2o_xs_oauth_init (H2OxsOauth *oauth)
{
    oauth->api_key = g_strdup ("FKQ82rveOOTIrzcEn6l4xKm1h");
    oauth->api_secret = g_strdup ("PXPOHfau2SeEQW5sVLjNIhP3vuuXJJokCFtKAHMADu7VZRm6pk");

    oauth->token = NULL;
    oauth->token_secret = NULL;

    oauth->user_id = NULL;
    oauth->screen_name = NULL;
}

static void
h2o_xs_oauth_class_init (H2OxsOauthClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);
    object_class->dispose = h2o_xs_oauth_dispose;
}

H2OxsOauth *
h2o_xs_oauth_new (void)
{
  return g_object_new (H2O_XS_OAUTH_TYPE, NULL);
}


/**
 * request_token_uri: "https://api.twitter.com/oauth/request_token"
 * auth url -> https://api.twitter.com/oauth/authorize?
 * oauth_token=XXX&
 * oauth_token_secret=YYY
 */

int
h2o_xs_oauth_request_token (H2OxsOauth *oauth, char *request_token_uri)
{
    char *req_url = NULL;
    char *req_hdr = NULL;
    char *http_hdr = NULL;
    char *reply;

    int  argc;
    char **argv = NULL;
    int size;

    argc = oauth_split_url_parameters (request_token_uri, &argv);
    oauth_sign_array2_process (&argc, &argv,
            NULL, //< postargs (unused)
            OA_HMAC,
            NULL, //< HTTP method (defaults to "GET")
            oauth->api_key, oauth->api_secret, NULL, NULL);

    // we split [x_]oauth_ parameters (for use in HTTP Authorization header)
    req_hdr = oauth_serialize_url_sep (argc, 1, argv, ", ", 6);
    // and other URL parameters
    req_url = oauth_serialize_url_sep (argc, 0, argv, "&", 1);

    oauth_free_array(&argc, &argv);

    http_hdr = g_strdup_printf ("Authorization: OAuth %s", req_hdr);

    reply = oauth_http_get2 (req_url, NULL, http_hdr);
    if (!reply) {
        if (req_url) free(req_url);
        if (req_hdr) free(req_hdr);
        if (http_hdr) g_free(http_hdr);
        return FALSE;
    } else {
        // parse reply - example:
        //"oauth_token=2a71d1c73d2771b00f13ca0acb9836a10477d3c56&oauth_token_secret=a1b5c00c1f3e23fb314a0aa22e990266"
        int rc;
        int i;
        char **rv = NULL;

        rc = oauth_split_url_parameters (reply, &rv);
        for (i=0; i<rc; i++) {
            gchar **param = g_strsplit (rv[i], "=", 2);
            if (!param[0] || !param[1])
                continue;

            if (g_strcmp0 (param[0], "oauth_token") == 0) {
                oauth->token = g_strdup (param[1]);
            } else if (g_strcmp0 (param[0], "oauth_token_secret") == 0) {
                oauth->token_secret = g_strdup (param[1]);
            }
            g_strfreev(param);
        }
        if(rv) free(rv);
    }

    if (req_url) free(req_url);
    if (req_hdr) free(req_hdr);
    if (http_hdr) g_free(http_hdr);
    if (reply) free(reply);

    return TRUE;
}


int
h2o_xs_oauth_verify (H2OxsOauth *oauth, char *request_token_uri, char *code)
{
    int  argc;
    char **argv = NULL;
    char *url = "%s?oauth_verifier=%s&oauth_token=%s&oauth_token_secret=%s";

    char *req_url = NULL;
    char *req_hdr = NULL;
    char *http_hdr= NULL;
    char *reply;

    char *params;

    params = g_strdup_printf (url, request_token_uri, code, oauth->token, oauth->token_secret);

    argc = oauth_split_url_parameters (params, &argv);

    oauth_sign_array2_process(&argc, &argv,
            NULL,
            OA_HMAC,
            NULL,
            oauth->api_key, oauth->api_secret, NULL, NULL);

    req_hdr = oauth_serialize_url_sep(argc, 1, argv, ", ", 6);
    req_url = oauth_serialize_url_sep(argc, 0, argv, "&", 1);

    oauth_free_array(&argc, &argv);

    http_hdr = g_strdup_printf ("Authorization: OAuth %s", req_hdr);
    reply = oauth_http_get2 (req_url, NULL, http_hdr);
    if (!reply) {
        if (req_url) free(req_url);
        if (req_hdr) free(req_hdr);
        if (http_hdr) g_free(http_hdr);
        return FALSE;
    } else {
        int rc;
        int i;
        char **rv = NULL;

        rc = oauth_split_url_parameters (reply, &rv);
        for (i=0; i<rc; i++) {
            gchar **param = g_strsplit (rv[i], "=", 2);
            if (!param[0] || !param[1])
                continue;

            if (g_strcmp0 (param[0], "oauth_token") == 0) {
                oauth->token = g_strdup (param[1]);
            } else if (g_strcmp0 (param[0], "oauth_token_secret") == 0) {
                oauth->token_secret = g_strdup (param[1]);
            } else if (g_strcmp0 (param[0], "user_id") == 0) {
                oauth->user_id = g_strdup (param[1]);
            } else if (g_strcmp0 (param[0], "screen_name") == 0) {
                oauth->screen_name = g_strdup (param[1]);
            }
            g_strfreev(param);
        }
        if(rv) free(rv);
    }

    if(reply) free(reply);
    if (req_url) free(req_url);
    if (req_hdr) free(req_hdr);
    if (http_hdr) g_free(http_hdr);

    if (!oauth->screen_name) {
        return FALSE;
    }

    return TRUE;
}

char*
h2o_xs_oauth_get_token (H2OxsOauth *oauth)
{
    return oauth->token;
}

char*
h2o_xs_oauth_get_token_secret (H2OxsOauth *oauth)
{
    return oauth->token_secret;
}

char*
h2o_xs_oauth_get_user_id (H2OxsOauth *oauth)
{
    return oauth->user_id;
}

char*
h2o_xs_oauth_get_screen_name (H2OxsOauth *oauth)
{
    return oauth->screen_name;
}

void
h2o_xs_oauth_set_api_key (H2OxsOauth *oauth, char *apikey)
{
    oauth->api_key = apikey;
}

void
h2o_xs_oauth_set_api_secret (H2OxsOauth *oauth, char *secret)
{
    oauth->api_secret = secret;
}

void
h2o_xs_oauth_set_token (H2OxsOauth *oauth, char *token)
{
    oauth->token = token;
}

void
h2o_xs_oauth_set_token_secret (H2OxsOauth *oauth, char *token_secret)
{
    oauth->token_secret = token_secret;
}

void
h2o_xs_oauth_set_screen_name (H2OxsOauth *oauth, char *screen_name, char *user_id)
{
    oauth->screen_name = screen_name;
    oauth->user_id = user_id;
}

char*
h2o_xs_oauth_get (H2OxsOauth *oauth, char *request_uri)
{
    char *req_url = NULL;
    char *req_hdr = NULL;
    char *http_hdr= NULL;
    char *reply;
    char *params;
    char *ret = NULL;
    int argc;
    char **argv = NULL;

    argc = oauth_split_url_parameters (request_uri, &argv);
    oauth_sign_array2_process(&argc, &argv,
            NULL,
            OA_HMAC,
            NULL,
            oauth->api_key, oauth->api_secret, oauth->token, oauth->token_secret);

    req_hdr = oauth_serialize_url_sep(argc, 1, argv, ", ", 6);
    req_url = oauth_serialize_url_sep(argc, 0, argv, "&", 1);

    oauth_free_array(&argc, &argv);

    http_hdr = g_strdup_printf ("Authorization: OAuth %s", req_hdr);
    reply = oauth_http_get2 (req_url, NULL, http_hdr);

    if (reply) {
        ret = g_strdup (reply);
        free (reply);
    }

    if (http_hdr) g_free (http_hdr);
    if (req_url) free(req_url);
    if (req_hdr) free(req_hdr);

    return ret;
}
