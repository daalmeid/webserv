#include "Delete.hpp"

namespace HTTP {

	Delete::Delete(void)
	: AMethod()
	{}

	Delete::~Delete(void)
	{}

	Delete::Delete(Delete const& cpy)
	: AMethod()
	{
		(void)cpy;
	};

	Delete&	Delete::operator=(Delete const& rhs)
	{
		(void)rhs;
		return *this;
	};

	void Delete::response(Client & client)
	{
		if (!client.childPid && _confCheck(client) == -1)
			return;
		cgi(client, "");
	};
}
