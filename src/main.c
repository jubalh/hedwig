#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strophe.h>
#include <unistd.h> /* getopt */
#include "users.h"
#include "xmpp.h"

struct list *pUsersInRoster = NULL;

void print_usage()
{
   printf("Options:\n");
   printf("-j JID\t\tset jid\n");
   printf("-p PASSWORD\tset password\n");
}

int main(int argc, char *argv[])
{
   xmpp_ctx_t *ctx;
   xmpp_conn_t *conn;
   xmpp_log_t *log;
   char *pJid = NULL;
   char *pPassword = NULL;

   printf("Hello World!\n");

   int opt;
   while ((opt = getopt(argc, argv, "j:p:")) != -1)
   {
      switch(opt)
      {
         case 'j':
            pJid = strdup(optarg);
            break;
         case 'p':
            pPassword = strdup(optarg);
            break;
      }
   }

   if (NULL == pJid || NULL == pPassword)
   {
      print_usage();
      return 1;
   }

   xmpp_initialize();

   //log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
   log = xmpp_get_default_logger(XMPP_LEVEL_ERROR);
   ctx = xmpp_ctx_new(NULL, log);

   conn = xmpp_conn_new(ctx);

   xmpp_conn_set_flags(conn, XMPP_CONN_FLAG_MANDATORY_TLS);
   xmpp_conn_set_keepalive(conn, 60, 1);
   xmpp_conn_set_jid(conn, pJid);
   xmpp_conn_set_pass(conn, pPassword);

   printf("connect\n");
   xmpp_connect_client(conn, NULL , 0, conn_handler, ctx);

   printf("run\n");
   xmpp_run(ctx);

   freeList(pUsersInRoster);
   free(pJid); free(pPassword);
   xmpp_conn_release(conn);
   xmpp_ctx_free(ctx);
   xmpp_shutdown();

   return 0;
}
