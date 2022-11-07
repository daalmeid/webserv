#include <cstdio>
#include <cerrno>
#include "webserv.hpp"

namespace ft
{
	/**
	 * @brief Writes an error to the stderr and returns.
	 *  Format: webserv: error: 'perror()'.
	 * @param ret return value.
	 * @return %ret.
	*/
	int	err( int ret )
	{
		perror("webserv: error");
		return ret;
	}

	/**
	 * @brief Writes an error to the stderr and returns.
	 *  Format: webserv: error: %str.
	 * @param ret - return value.
	 * @param str - error string.
	 * @return %ret.
	*/
	int	err( int ret, char const* str )
	{
		std::cerr << "webserv: error: " << str << std::endl;
		return ret;
	}

	/**
	 * @brief Writes an error to the stderr and returns.
	 *  Format: webserv: %err: %str.
	 * @param ret return value.
	 * @param err error type.
	 * @param str error string.
	 * @return %ret.
	*/
	int	err( int ret, char const* err, char const* str )
	{
		std::cerr << "webserv: " << err << ": " << str << std::endl;
		return ret;
	}
}
