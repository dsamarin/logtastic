#ifndef LOG_FORMAT_H
#define LOG_FORMAT_H

enum format_token_type {
	T_CHARACTER,
	T_VARIABLE
};

enum format_token_variable {
	V_UNKNOWN,
	V_REMOTE_ADDR,
	V_REMOTE_USER,
	V_TIME_LOCAL,
	V_REQUEST,
	V_STATUS,
	V_BODY_BYTES_SENT,
	V_HTTP_REFERER,
	V_HTTP_USER_AGENT
};

struct format_token {
	enum format_token_type type;
	union {
		char ch;
		enum format_token_variable var;
	} data;
};

struct format_token * parse_format_token (char **input);

#endif
