#include <glib.h>
#include <ctype.h>
#include <stdlib.h>

#include "log_format.h"

#define I(x) g_intern_static_string(x)

static struct format_token * create_format_token_ch (char ch) {
	struct format_token *token = malloc (sizeof *token);
	if (token) {
		token->type = T_CHARACTER;
		token->data.ch = ch;
		token->next = NULL;
	}
	return token;
}

static struct format_token * create_format_token_var (enum format_token_variable var) {
	struct format_token *token = malloc (sizeof *token);
	if (token) {
		token->type = T_VARIABLE;
		token->data.var = var;
		token->next = NULL;
	}
	return token;
}

static GHashTable * create_variable_hash(void) {
	GHashTable *hash = g_hash_table_new (NULL, NULL);

	static enum format_token_variable remote_addr = V_REMOTE_ADDR;
	static enum format_token_variable remote_user = V_REMOTE_USER;
	static enum format_token_variable time_local = V_TIME_LOCAL;
	static enum format_token_variable request = V_REQUEST;
	static enum format_token_variable status = V_STATUS;
	static enum format_token_variable body_bytes_sent = V_BODY_BYTES_SENT;
	static enum format_token_variable http_referer = V_HTTP_REFERER;
	static enum format_token_variable http_user_agent = V_HTTP_USER_AGENT;

	g_hash_table_insert (hash, (gpointer) I("remote_addr"), &remote_addr);
	g_hash_table_insert (hash, (gpointer) I("remote_user"), &remote_user);
	g_hash_table_insert (hash, (gpointer) I("time_local"), &time_local);
	g_hash_table_insert (hash, (gpointer) I("request"), &request);
	g_hash_table_insert (hash, (gpointer) I("status"), &status);
	g_hash_table_insert (hash, (gpointer) I("body_bytes_sent"), &body_bytes_sent);
	g_hash_table_insert (hash, (gpointer) I("http_referer"), &http_referer);
	g_hash_table_insert (hash, (gpointer) I("http_user_agent"), &http_user_agent);

	return hash;
}

static struct format_token * parse_format_token_variable (char **input) {
	gsize length = 0;
	char *name = NULL;
	char *begin = NULL;
	char *format = NULL;
	struct format_token *token = NULL;
	enum format_token_variable *resolved, type;

	static GHashTable *variable_lookup = NULL;
	if (!variable_lookup) {
		variable_lookup = create_variable_hash ();
	}

	if (input && *input) {
		format = *input;
		if (*format == '$') {
			format++;
			begin = format;
			while (isalnum (*format) || *format == '_') {
				format++;
				length++;
			}
			name = g_strndup (begin, length);

			resolved = g_hash_table_lookup (variable_lookup, g_intern_string (name));
			type = V_UNKNOWN;
			if (resolved) {
				type = *resolved;
			}

			g_free (name);

			token = create_format_token_var (type);
			*input = format;
		}
	}

	return token;
}

static struct format_token * parse_format_token (char **input) {
	struct format_token *token = NULL;

	if (input && *input) {
		char *format = *input;

		switch (*format) {
		case '$': return parse_format_token_variable (input);
		case '\0': return NULL;
		default:
			token = create_format_token_ch (*format);
			(*input)++;
		}
	}

	return token;
}


struct format_token * parse_log_format (char *format) {
	struct format_token *head = NULL;
	struct format_token *tail = NULL;
	struct format_token *item = NULL;

	while ((item = parse_format_token (&format))) {
		if (tail) {
			tail->next = item;
		} else {
			head = item;
		}
		tail = item;
	}

	return head;
}

