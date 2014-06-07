#ifndef H2O_XS_SOCIAL
#define H2O_XS_SOCIAL

#include "h2oxs-win.h"
#include "h2oxs-oauth.h"
#include <gtk/gtk.h>
#include <json-glib/json-glib.h>


/** Twitter api **/
void h2o_xs_social_twitter_account_popup (GtkWidget *widget, H2OxsWindow *win);

// Return a list of H2OxsOauth with twitter registered accounts
GList* h2o_xs_social_twitter_accounts ();

char* h2o_xs_social_twitter_new (H2OxsOauth *oauth);
int h2o_xs_social_twitter_confirm (H2OxsOauth *oauth, gchar *code);
void h2o_xs_social_twitter_store (H2OxsOauth *oauth);

void h2o_xs_social_twitter_home (H2OxsOauth *oauth, GtkWidget *listbox);

#endif // H2O_XS_SOCIAL
