// vim: noexpandtab:ts=4:sts=4:sw=4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strophe.h>
#include <unistd.h> /* access */
#include "parser.h"
#include "global.h"
#include "color.h"

#define ALLOC_SIZE 512

static char * exec_return_output(char * const command)
{
	FILE *out_file;
	char *output_buffer;
	size_t output_size = 0;
	size_t off = 0;

	out_file = popen(command, "r");
	if (!out_file)
	{
		fprintf(stderr, "Error while popen() with command: %s\n", command);
		return NULL;
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

	fclose(out_file);

	// in case of extra newline, remove it
	int len = strlen(output_buffer);
	if ('\n' == output_buffer[len - 1])
		output_buffer[len - 1] = '\0';

	return output_buffer;
}

static void send(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	char *id = xmpp_uuid_gen(ctx);
	xmpp_stanza_t *message_st = xmpp_message_new(ctx, "chat", jid, id);
	xmpp_message_set_body(message_st, message);
	xmpp_send(conn, message_st);
	xmpp_stanza_release(message_st);
	xmpp_free(ctx, id);
}

static void parse_exec(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	char *output;

	output = exec_return_output(message);
	printf("Output:\n%s%s%s\n", CLR_CYAN, output, CLR_DEF);

	send(conn, ctx, output, jid);

	free(output);
}

static void parse_cmd(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	char cmd[2048];
	snprintf(cmd, 2048, "%s/%s", g_wd, message);

	char *output;
	if (0 == access(cmd, X_OK))
	{
		printf("Running %s%s%s\n", CLR_GREEN, cmd, CLR_DEF);
		output = exec_return_output(cmd);
		printf("Output:\n%s%s%s\n", CLR_CYAN, output, CLR_DEF);
	}
	else
	{
		printf("No executable file: %s%s%s\n", CLR_YELLOW, cmd, CLR_DEF);
		output = strdup("Command not known");
	}

	send(conn, ctx, output, jid);

	free(output);
}

void parse(xmpp_conn_t * const conn, xmpp_ctx_t *ctx, char *message, const char *jid)
{
	if (message && (strlen(message) > 4))
	{
		if (message[0] == '!' &&
				message[1] == 'e' &&
				message[2] == 'x' &&
				message[3] == 'e' &&
				message[4] == 'c')
		{
			if (g_allow_exec)
			{
				parse_exec(conn, ctx, &message[5], jid);
			}
		}
		else if (message[0] == '!' &&
				message[1] == 'c' &&
				message[2] == 'm' &&
				message[3] == 'd' &&
				message[4] == ' ')
		{
			parse_cmd(conn, ctx, &message[5], jid);
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
