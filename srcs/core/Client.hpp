#pragma once

#include "webserv.hpp"
#include "Message.hpp"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace HTTP
{
	#define TMP_BUFF 8000

	enum
	{
		STATUS_LINE,
		HEADER_FIELDS,
		BODY_CONTENT,
		OK,
	};

	class Client
	{
	public:
		~Client( void );
		Client( void );

		Message		req;
		Message		res;
		int			fd;
		struct \
		sockaddr_in	ai;
		double		timestamp;
		JSON::Node *config;

		int update( void );
		bool ok( void );

	private:
		int state;
	};
}
