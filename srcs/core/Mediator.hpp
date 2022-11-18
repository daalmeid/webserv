#pragma	once

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include "HTTPReq.hpp"
#include "webserv.hpp"

namespace ft {

	class Mediator {

		public:
			Mediator(void);

			void		method_choice(HTTPReq const& req, int client_fd);
			std::string	get_date(time_t now);
		private:
			
			void	get(HTTPReq const& req, int client_fd);
			void	post(HTTPReq const& req, int client_fd);
			void	del(HTTPReq const& req,int client_fd);

			void	get_file(HTTPReq const& req, HTTPReq& resp, std::string& path);
			void	content_encoding(std::fstream & ifs, int client_fd, HTTPReq& resp);
	};
}