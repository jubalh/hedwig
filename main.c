#include <stdio.h>
#include <string.h>
#include <strophe.h>

#define JID "test@jabber.org"
#define PASSWORD "password"

int message_handler(xmpp_conn_t * const conn,
                    xmpp_stanza_t * const stanza,
                    void * const userdata)
{
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

       xmpp_handler_add(conn, message_handler, NULL, "presence", NULL, ctx);

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

int main(int argc, char *argv[])
{
   xmpp_ctx_t *ctx;
   xmpp_conn_t *conn;
   xmpp_log_t *log;

   printf("Hello World!\n");

   xmpp_initialize();

   //log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
   log = xmpp_get_default_logger(XMPP_LEVEL_ERROR);
   ctx = xmpp_ctx_new(NULL, log);

   conn = xmpp_conn_new(ctx);

   xmpp_conn_set_flags(conn, XMPP_CONN_FLAG_MANDATORY_TLS);
   xmpp_conn_set_keepalive(conn, 60, 1);
   xmpp_conn_set_jid(conn, JID);
   xmpp_conn_set_pass(conn, PASSWORD);

   printf("connect\n");
   xmpp_connect_client(conn, "" , 0, conn_handler, ctx);

   printf("run\n");
   xmpp_run(ctx);

   xmpp_conn_release(conn);
   xmpp_ctx_free(ctx);
   xmpp_shutdown();

   return 0;
}
