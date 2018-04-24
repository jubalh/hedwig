// vim: noexpandtab:ts=4:sts=4:sw=4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strophe.h>
#include "parser.h"

#define ALLOC_SIZE 512

void parse_run(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	FILE *out_file;
	char *output_buffer;
	size_t size_read;
	size_t output_size = 0;
	size_t off = 0;

	out_file = popen(&message[5], "r");
	if (!out_file)
	{
		fprintf(stderr, "Error whilte popen() from message: %s\n", message);
		return;
	}

	output_buffer = malloc(ALLOC_SIZE);
	output_buffer[0] = '\0';
	output_size = ALLOC_SIZE;

	bool done = false;
	while (!done) {
		if (off >= output_size) {
			output_buffer = realloc(output_buffer, output_size*2);
			output_size *= 2;
		}

		size_t left = output_size - off;
		size_t r = fread(output_buffer + off, 1, left, out_file);
		if (r < left) {
			if (feof(out_file)) {
				done = true;
			} else {
				fprintf(stderr, "Error in fread\n");
			}
		}
		off += r;
	}

	printf("output:\n%s\n", output_buffer);
	fclose(out_file);

	char *id = xmpp_uuid_gen(ctx);
	xmpp_stanza_t *message_st = xmpp_message_new(ctx, "chat", jid, id);
	xmpp_message_set_body(message_st, output_buffer);
	xmpp_send(conn, message_st);
	xmpp_stanza_release(message_st);
	xmpp_free(ctx, id);
}

void parse(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	if (message && (strlen(message) > 4))
	{
		if (message[0] == '!' &&
				message[1] == 'r' &&
				message[2] == 'u' &&
				message[3] == 'n' &&
				message[4] == ' ')
		{
			parse_run(conn, ctx, message, jid);
		}
		else if (message[0] == '!' &&
				message[1] == 'q' &&
				message[2] == 'u' &&
				message[3] == 'i' &&
				message[4] == 't')
		{
			printf("Shutting down\n");
			xmpp_disconnect(conn);
		}
	}
}
