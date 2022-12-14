#include "CGI.hpp"

namespace HTTP
{	
	CGI::~CGI( void )
	{
		int i = 0;
		while(env[i] != NULL)
			delete[] env[i++];
		delete[] env;
		
		i = 0;
		while(args[i] != NULL)
			delete[] args[i++];
		delete[] args;
	}

	CGI::CGI( void )
	{}

	CGI::CGI( CGI const& other )
	{
		*this = other;
	}

	CGI::CGI(Message const& req)
	{
		env = new char*[9];
		std::vector<std::string>	vec;

		vec.reserve(8);
		vec.push_back("PATH_INFO=/usr/bin/python3");
		if (req.getField("content-type"))
			vec.push_back("CONTENT_TYPE=" + *req.getField("content-type"));
		else
			vec.push_back("CONTENT_TYPE=");
		if (req.getField("content-length"))
			vec.push_back("CONTENT_LENGTH=" + *req.getField("content-length"));
		else
			vec.push_back("CONTENT_LENGTH=");
		if (req.getField("user-agent"))
			vec.push_back("HTTP_USER_AGENT=" + *req.getField("user-agent"));
		else
			vec.push_back("HTTP_USER_AGENT=");
		vec.push_back("SCRIPT_FILENAME=/nfs/homes/rafernan/Desktop/webserv" + req.getMethod()[1]);
		vec.push_back("REQUEST_METHOD=" + req.getMethod()[0]);
		vec.push_back("SERVER_SOFTWARE=Webserv/0.4");
		if (req.getMethod().size() >= 4)
			vec.push_back("QUERY_STRING=" + req.getMethod()[3]);
		else
			vec.push_back("QUERY_STRING=");

		for (int i = 0; i < 8; i++) {
			env[i] = new char[vec[i].size() + 1];
			memcpy(env[i], vec[i].c_str(), vec[i].size());
			env[i][vec[i].size()] = '\0';
		}
		env[8] = NULL;
		
		std::string temp = vec[4].substr(vec[4].find_first_of('/'));
		args = new char*[3];
		args[0] = new char[strlen("/usr/bin/python3") + 1];
		memcpy(args[0], "/usr/bin/python3", strlen("/usr/bin/python3"));
		args[0][strlen("/usr/bin/python3")] = '\0';
		args[1] = new char[temp.size() + 1];
		memcpy(args[1], temp.c_str(), temp.size());
		args[1][temp.size()] = '\0';
		args[2] = NULL;
	}

	CGI &
	CGI::operator=( CGI const& rhs )
	{
		DEBUG2("DO NOT CALL THIS COPY OPERATOR");
		(void) rhs;
		return *this;
	}

	char**	CGI::getEnv(void)
	{
		return env;
	};

	char**	CGI::getArgs(void)
	{
		return args;
	};
}
