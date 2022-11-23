#include "HTTPSocks.hpp"
#include "HTTPServer.hpp"

namespace ft
{
	HTTPSocks::~HTTPSocks( void )
	{
		this->clear();
	}

	HTTPSocks::HTTPSocks( void )
	{}

	HTTPSocks::HTTPSocks( HTTPSocks const& other )
	{
		*this = other;
	}

	HTTPSocks &
	HTTPSocks::operator=( HTTPSocks const& rhs )
	{
		this->clear();
		this->list = rhs.list;
		return *this;
	}

	static int insertInitSock( t_sock_info & sock_info )
	{
		int			addrlen = sizeof(sock_info.addr);
		int			enable = 1;

		sock_info.fd = socket(AF_INET, SOCK_STREAM, 0);
		if (sock_info.fd == -1)
			return (-1);
		sock_info.addr.sin_family = AF_INET;
		sock_info.addr.sin_port = htons(sock_info.port);
		sock_info.addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (setsockopt(sock_info.fd, SOL_SOCKET, SO_REUSEADDR, 
			&enable, sizeof(int)) == -1)
		{
			close(sock_info.fd);
			return (-1);
		}
		if (bind(sock_info.fd, 
			(struct sockaddr *)&sock_info.addr, addrlen) == -1)
		{
			close(sock_info.fd);
			return (-1);
		}
		if (listen(sock_info.fd, S_MAX_CLIENT) == -1)
		{
			close(sock_info.fd);
			return (-1);
		}
		return (0);
	}

	t_sock_info const*
	HTTPSocks::insert( JsonToken * block )
	{
		ft::JsonToken * port_token;
		t_sock_info		tmp;
		t_sock_info const*	match;

		memset(&tmp, 0, sizeof(tmp));
		port_token = block->find_first("listen");
		if (!port_token)
			return 0;
		tmp.port = Json::getIntegerOf(port_token);
		tmp.conf = block;
		match = this->findByPort(tmp.port);
		if (match)
		{
			if (match->conf == block)
			{
				ft::err(1, "duplicate port in same block");
				return 0;
			}
			return match;
		}
		if (insertInitSock(tmp) != -1)
		{
			DEBUG2("listening .. " << tmp.port);
			return &(*list.insert(list.begin(), tmp));
		}
		ft::err(1);
		return 0;
	}

	t_sock_info *
	HTTPSocks::findByFd( int sock_fd )
	{
		for (std::list<t_sock_info>::iterator it = list.begin();
			it != list.end(); it++)
		{
			if (it->fd == sock_fd || it->clients.count(sock_fd))
				return &(*it);
		}
		return NULL;
	}

	t_sock_info const*
	HTTPSocks::findByPort( int port ) const
	{
		for (std::list<t_sock_info>::const_iterator it = list.begin();
			it != list.end(); it++)
		{
			if (it->port == port)
				return &(*it);
		}
		return NULL;
	}

	void
	HTTPSocks::clear( void )
	{
		if (list.size() > 0)
		{
			for (std::list<t_sock_info>::iterator it = list.begin();
				it != list.end(); it++)
			{
				close(it->fd);
			}
			list.clear();
		}
	}
}
