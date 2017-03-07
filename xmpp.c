#include <stdio.h>
#include <strophe.h>
#include "users.h"

extern struct list *pUsersInRoster; // defined in main.c

int message_handler(xmpp_conn_t * const conn,
                    xmpp_stanza_t * const stanza,
                    void * const userdata)
{
   char *message, *from, *bare_from;
   xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;

   if(!xmpp_stanza_get_child_by_name(stanza, "body")) return 1;
   if(!strcmp(xmpp_stanza_get_attribute(stanza, "type"), "error")) return 1;

   message = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "body"));

   printf("Received from %s: %s\n", xmpp_stanza_get_attribute(stanza, "from"), message);

   // TODO: continue here
   from = xmpp_stanza_get_attribute(stanza, "from");
   bare_from = xmpp_jid_bare(ctx, from);
   if (isInList(pUsersInRoster, bare_from))
   {
      printf("Ok!\n");
      //parse(message);
   }
   else
   {
      printf("Ignoring: %s not in roster.\n", bare_from);
   }

   xmpp_free(ctx, message);
   xmpp_free(ctx, bare_from);
   xmpp_stop(ctx); // TODO: for testing only

   return 1;
}

void roster_handler(xmpp_conn_t * const conn,
                    xmpp_stanza_t * const stanza,
                    void * const userdata)
{
   xmpp_stanza_t *query, *item;
   const char *type, *name;

   type = xmpp_stanza_get_type(stanza);
   if (strcmp(type, "error") == 0)
      fprintf(stderr, "ERROR: query failed\n");
   else {
      query = xmpp_stanza_get_child_by_name(stanza, "query");

      printf("Roster:\n");
      for (item = xmpp_stanza_get_children(query); item; item = xmpp_stanza_get_next(item))
      {
         printf("\t %s sub=%s\n",
            xmpp_stanza_get_attribute(item, "jid"),
            xmpp_stanza_get_attribute(item, "subscription"));

         addToList(&pUsersInRoster, xmpp_stanza_get_attribute(item, "jid"));

         printf("END OF LIST\n");
      }
   }
}

void conn_handler(xmpp_conn_t * const conn,
              const xmpp_conn_event_t event,
              const int error,
              xmpp_stream_error_t * const stream_error,
              void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    xmpp_stanza_t *iq, *query, *pres;

    if (event == XMPP_CONN_CONNECT)
    {
       printf("DEBUG: connected\n");

       xmpp_handler_add(conn, message_handler, NULL, "message", NULL, ctx);

       // send presence
       //pres = xmpp_stanza_new(ctx);
       //xmpp_stanza_set_name(pres, "presence");
       pres = xmpp_presence_new(ctx);
       xmpp_send(conn, pres);
       xmpp_stanza_release(pres);

       // send roster request
       iq = xmpp_stanza_new(ctx);
       xmpp_stanza_set_name(iq, "iq");
       xmpp_stanza_set_type(iq, "get");
       xmpp_stanza_set_id(iq, "roster1");

       query = xmpp_stanza_new(ctx);
       xmpp_stanza_set_name(query, "query");
       xmpp_stanza_set_ns(query, XMPP_NS_ROSTER);

       xmpp_stanza_add_child(iq, query);

       xmpp_stanza_release(query);
       xmpp_id_handler_add(conn, roster_handler, "roster1", ctx);

       xmpp_send(conn, iq);
       xmpp_stanza_release(iq);
    }
    else
    {
       printf("DEBUG: connection failed\n");
       xmpp_stop(ctx);
    }
}
