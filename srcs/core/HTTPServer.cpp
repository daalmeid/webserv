#include <cstdlib>
#include "webserv.hpp"
#include "HTTPServer.hpp"

namespace ft
{
	HTTPServer::~HTTPServer( void )
	{}

	HTTPServer::HTTPServer( void )
	{}

	HTTPServer::HTTPServer( HTTPServer const& other )
	{
		*this = other;
	}

	HTTPServer &
	HTTPServer::operator=( HTTPServer const& rhs )
	{
		DEBUG2("called non-implemented function: HTTPServer::operator=( HTTPServer const& rhs )");
		(void) rhs;
		return *this;
	}

	int
	HTTPServer::init( void )
	{
		if (!socks.insert(8000))
			return -1;

		if (!socks.insert(4000))
			return -1;

		for (std::list<ft::t_sock_info>::iterator it = socks.list.begin();
			it != socks.list.end(); it++)
		{DEBUG2("listening on port " << it->port);}
		epoll.init(socks);
		return 0;
	}

	int
	HTTPServer::init( char const* filepath )
	{
		DEBUG2("called non-implemented function: HTTPServer::init( char* filepath )");
		(void) filepath;
		return (-1);
	}
	void
	HTTPServer::loop( void )
	{
		if (socks.list.empty())
			exit(err(1, "logic error", "no sockets available"));
		while (1)
		{
			int ev_count = epoll.wait();
			DEBUG2("[" << ev_count << "] ready events");
			int ev_socket;
			for (int i = 0; i < ev_count; i++)
			{
				ev_socket = epoll.events[i].data.fd;
				if (socks.find(ev_socket))
				{
					if((ev_socket = accept(ev_socket, NULL, 0)) == -1)
						err(-1, "accept()");
					if (epoll.insert(ev_socket) == -1)
						err(-1, "insert()");
				}
				else
					/*
						HTTPReq
						parse the raw text received into a struct/class
					*/
					ft_handle(i);
				//DEBUG2("connection received");
				// send(fd, 
				// 	"HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!",
				// 	74, 0);
				//close (fd);
			}
		}
	}

	void	HTTPServer::ft_handle(int i) {
		/*
			HTTPRes
			build the http response
		*/
		static char buffer[30000] = {0};
		const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

		int valread = recv(epoll.events[i].data.fd, buffer, 30000, 0);
		if (valread == -1)
			DEBUG2("client disconnect");
		DEBUG2(buffer);
		send(epoll.events[i].data.fd, hello, strlen(hello), 0);
		DEBUG2("message sent");
		if (epoll.erase(i) == -1)
			err(-1);
	}
}