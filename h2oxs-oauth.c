#include "h2oxs-oauth.h"
#include <rest/oauth-proxy.h>
#include <json-glib/json-glib.h>

struct _H2OxsOauth
{
    GObject parent;

    char *api_key;
    char *api_secret;
    char *token;
    char *token_secret;

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

gboolean
h2o_xs_oauth_request_token (H2OxsOauth *oauth, char *api_url)
{
  RestProxy *proxy;
  GError *error = NULL;

  proxy = oauth_proxy_new (oauth->api_key,
                           oauth->api_secret,
                           api_url, FALSE);

  if (!oauth_proxy_request_token (OAUTH_PROXY (proxy), "oauth/request_token", "oob", &error)) {
      g_printf ("Cannot get request token: %s\n", error->message);
      g_object_unref (proxy);
      return FALSE;
  }

  oauth->token = g_strdup (oauth_proxy_get_token (OAUTH_PROXY (proxy)));
  oauth->token_secret = g_strdup (oauth_proxy_get_token_secret (OAUTH_PROXY (proxy)));

  g_object_unref (proxy);
  return TRUE;
}


int
h2o_xs_oauth_verify (H2OxsOauth *oauth, char *api_url, char *code)
{
    RestProxy *proxy;
    RestProxyCall *call;
    GError *error = NULL;

    proxy = oauth_proxy_new_with_token (oauth->api_key,
            oauth->api_secret,
            oauth->token,
            oauth->token_secret,
            api_url, FALSE);

    if (!oauth_proxy_access_token (OAUTH_PROXY (proxy), "oauth/access_token", code, &error)) {
        g_printf ("Cannot get access token: %s\n", error->message);
        g_object_unref (proxy);
        return FALSE;
    }

    oauth->token = g_strdup (oauth_proxy_get_token (OAUTH_PROXY (proxy)));
    oauth->token_secret = g_strdup (oauth_proxy_get_token_secret (OAUTH_PROXY (proxy)));

    call = rest_proxy_new_call (proxy);
    rest_proxy_call_set_function (call, "1.1/account/verify_credentials.json");
    rest_proxy_call_set_method (call, "GET");

    if (!rest_proxy_call_sync (call, &error)) {
        g_printf ("Cannot make call: %s\n", error->message);
        g_object_unref (call);
        g_object_unref (proxy);
        return FALSE;
    }

    JsonParser *parser = json_parser_new ();

    json_parser_load_from_data (parser,
                                rest_proxy_call_get_payload (call),
                                rest_proxy_call_get_payload_length (call),
                                NULL);

    JsonNode *root = json_parser_get_root (parser);
    JsonObject *obj = json_node_get_object (root);

    oauth->screen_name = g_strdup (json_object_get_string_member (obj, "screen_name"));

    g_object_unref (call);
    g_object_unref (proxy);

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
h2o_xs_oauth_set_screen_name (H2OxsOauth *oauth, char *screen_name)
{
    oauth->screen_name = screen_name;
}
