#pragma	once

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <utility>
#include <algorithm>
#include "webserv.hpp"

namespace HTTP {

	class Message {

		public:
			Message(void);
			
			/**
			 * @brief Builds an object based on the incoming request, dividing it
			 * in a map of the headers and a string with the request.
			 * @param request request received by the server.
			*/
			Message(Message const& cpy);
			~Message(void);
			Message&	operator=(Message const& rhs);
			
			/**
			 * @brief Prints the content of the header map (debug use).
			*/
			void	print_map(void) const;

			int							init(std::string & req);

			std::vector<std::string>	get_method(void) const;
			std::string					get_head_val(std::string const& key) const;
			void						add(std::string key, std::string value);
			void						addToVal(std::string key, std::string value_to_add);
			void						create_vec_method(std::string const& str);
			std::string					response_string(void);

			std::string 				getBody(void) const;
			void						setBody(std::string bod);

			JSON::JsonToken * conf;
			std::map<std::string, std::string>	headers; //Max size of the headers section: 8k
		private:

			std::vector<std::string>			method; //Max size: 8k
			std::string							_body;
			
			/**
			 * @brief Helper function, creates a key-value pair based on one header
			 * line of the original request string and adds it to the header map.
			 * @param str string to be split in key and value pair.
			*/
			void	pair_create_add(std::string str);
			
			/**
			 * @brief Removes starting and trailing whitespaces in a header string.
			 * @param socks Contains the opened sockets where connections will come
			 * from.
			 * @return
			 * Still working on return. Currently returns an empty string if the
			 * original is also an empty string, otherwise returns a copy of the
			 * string without the previously mentioned whitespaces.
			*/
			std::string	wp_trimmer(std::string const& str);
			
	};

	int	ftStoi(std::string str);
}