#include <gtk/gtk.h>
#include <stdio.h>

#include "h2oxs-oauth.h"


int main (int argc, char *argv[])
{
    H2OxsOauth *oauth = h2o_xs_oauth_new ();
    char code[100];

    h2o_xs_oauth_set_api_key (oauth, "FKQ82rveOOTIrzcEn6l4xKm1h");
    h2o_xs_oauth_set_api_secret (oauth, "PXPOHfau2SeEQW5sVLjNIhP3vuuXJJokCFtKAHMADu7VZRm6pk");

    h2o_xs_oauth_request_token (oauth, "https://api.twitter.com/oauth/request_token");
    g_printf ("url: https://api.twitter.com/oauth/authorize?oauth_token=%s&oauth_token_secret=%s\n",
              h2o_xs_oauth_get_token(oauth),
              h2o_xs_oauth_get_token_secret(oauth));

    g_printf ("\ncode: ");
    scanf("%s", &code);
    h2o_xs_oauth_verify (oauth, "https://api.twitter.com/oauth/access_token", code);

    g_printf ("OUT: %s\n", h2o_xs_oauth_get_screen_name (oauth));

    char *home = h2o_xs_oauth_get (oauth, "https://api.twitter.com/1.1/statuses/home_timeline.json");

    g_printf ("\n%s\n", home);

    g_object_unref (G_OBJECT (oauth));
}
