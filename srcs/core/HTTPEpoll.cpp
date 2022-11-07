/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPEpoll.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daalmeid <daalmeid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:31:27 by daalmeid          #+#    #+#             */
/*   Updated: 2022/11/07 14:53:42 by daalmeid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPEpoll.hpp"

namespace ft {

	HTTPEpoll::HTTPEpoll(void) {
		
	};

	HTTPEpoll::HTTPEpoll(HTTPEpoll const& cpy) {
		(void)cpy;
	};

	int	HTTPEpoll::init(HTTPSocks const& socks) {
		this->size = EPOLL_SIZE;
		this->fd = epoll_create(EPOLL_SIZE);
		if (this->fd  == -1)
			return err(-1, "error in epoll_create()");
		for (std::list<t_sock_info>::const_iterator it = socks.list.begin();
			it != socks.list.end(); it++)
		{
			if (insert(it->fd) == -1)
				return err(-1, "error in insert()");
		}
		return 0;
	};

	int	HTTPEpoll::insert(int sofd) {
		epoll_event		ev;

		ev.events = EPOLLIN;
		ev.data.fd = sofd;
		int	flags = fcntl(sofd, F_GETFL, 0);
		if (flags == -1)
			return -1;
		if (fcntl(sofd, F_SETFL, flags | O_NONBLOCK )  == -1)
			return -1;
		return (epoll_ctl(this->fd, EPOLL_CTL_ADD, sofd, &ev));
	}

	int	HTTPEpoll::erase(int ev_index) {
		if (epoll_ctl(this->fd, EPOLL_CTL_DEL,
				this->events[ev_index].data.fd, NULL) == -1)
			return -1;
		close(this->events[ev_index].data.fd);
		return 0;
	};
	
	int	HTTPEpoll::wait(void) {

		int		ev_count = epoll_wait(this->fd, this->events,
			EPOLL_SIZE, EPOLL_TIMEOUT);
		if (ev_count == -1)
			DEBUG2("error in epoll_wait()");
		return ev_count;
	}
}