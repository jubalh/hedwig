// vim: noexpandtab:ts=4:sts=4:sw=4

#ifndef XMPP_H
#define XMPP_H

int message_handler(xmpp_conn_t * const conn,
		xmpp_stanza_t * const stanza,
		void * const userdata);

void roster_handler(xmpp_conn_t * const conn,
		xmpp_stanza_t * const stanza,
		void * const userdata);

void conn_handler(xmpp_conn_t * const conn,
		const xmpp_conn_event_t event,
		const int error,
		xmpp_stream_error_t * const stream_error,
		void * const userdata);

#endif // XMPP_H
