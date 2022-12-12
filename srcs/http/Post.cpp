#include "Post.hpp"

namespace HTTP {

	Post::Post(void)
	: AMethod()
	{}

	Post::~Post(void)
	{}

	Post::Post(Post const& cpy)
	: AMethod()
	{
		(void)cpy;
	};

	Post&	Post::operator=(Post const& rhs)
	{
		(void)rhs;
		return *this;
	};

	void		Post::operator()(Client & client)
	{
		cgi(client);
	};
}