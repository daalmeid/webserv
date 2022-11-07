#include <map>
#include <iostream>
#include <string>
#include <cstring>
#include <utility>


namespace ft {

	class HTTPReq {

		public:
			HTTPReq(void);
			HTTPReq(char* request);
			HTTPReq(HTTPReq const& cpy);
			~HTTPReq(void);
			HTTPReq&	operator=(HTTPReq const& rhs);

			void		print_map(void) const;
			void		pair_create_add(std::string str);
			std::string	wp_trimmer(std::string const& str);

		private:
			std::map<std::string, std::string>	headers;
			std::string							method;
	};

	HTTPReq::HTTPReq(void) {

	}

	HTTPReq::HTTPReq(char* request) {
		std::string	str(request);
		size_t		str_start = str.find("\n");
		
		method = str.substr(0, str_start++);
		str = &(request[str_start]);
		str_start = 0;
		
		for (size_t i = 0; str[i] != '\0' ; i++) {
			if (str[i] == '\n') {
				std::string	new_header(str.substr(str_start, i - str_start - 1));
				pair_create_add(new_header);
				str_start = i + 1;
				if (str[i + 1] == '\r')
					break;
			}
		}
		if (str[str_start] == '\r')
			headers.insert(std::make_pair("body", wp_trimmer(str.substr(str_start + 2))));
	}
}