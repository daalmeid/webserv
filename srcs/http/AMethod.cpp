#include "AMethod.hpp"
#include "Server.hpp"

namespace HTTP
{
	AMethod::AMethod(void)
	{};

	AMethod::AMethod(AMethod const& cpy)
	{
		(void)cpy;
	};

	AMethod::~AMethod(void)
	{};

	AMethod& AMethod::operator=(AMethod const& rhs)
	{
		(void)rhs;
		return *this;
	};

	void AMethod::cgi(Client & client, std::string const& path)
	{
		int	bytes = 0;
		std::string const& body = client.req.body;

		if (client.childPid == 0)
		{
			if (pipe(client.clientPipe) == -1)
				return client.error(500, true);
			if (fcntl(client.clientPipe[1], F_SETFL, O_NONBLOCK )  == -1)
				return client.error(500, true);
			client.childPid = fork();
			if (client.childPid == -1)
				return client.error(500, true);
			else if (client.childPid == 0)
			{	
				close(client.clientPipe[1]);
				CGI	test(client, path);
				while (client.server->getParent() != NULL)
					client.server = client.server->getParent();
				delete client.server;
				if (dup2(client.clientPipe[0], STDIN_FILENO) == -1)
					exit(EXIT_FAILURE);
				close (client.clientPipe[0]);
				if (dup2(client.fd, STDOUT_FILENO) == -1)
					exit(EXIT_FAILURE);
#ifndef DEBUG_MODE
				close(STDERR_FILENO);
#endif
				execve("/usr/bin/python3", test.getArgs(), test.getEnv());
				exit (EXIT_FAILURE);
			}
			else 
			{
				close(client.clientPipe[0]);
				client.clientPipe[0] = 0;
				bytes = write(client.clientPipe[1], body.c_str(), body.size());
				if (bytes > 0)
				{
					client.cgiSentBytes += bytes;
					client.req.body.erase(0, bytes);
				}
				client.state = CGI_PIPING;
				if ((size_t)client.cgiSentBytes == client.req.content_length)
				{
					close(client.clientPipe[1]);
					client.clientPipe[1] = 0;
					client.state = CGI_FINISHED;
				}
			}
		}
		else
		{
			bytes = write(client.clientPipe[1], body.c_str(), body.size());
			if (bytes > 0)
			{
				client.cgiSentBytes += bytes;
				client.req.body.erase(0, bytes);
			}
			else
			{
				/*Due to being non-blocking fds, the pipe buffer might be still full when the server
					gets here, this state allows it to return to the loop without erasing stuff.*/
				client.state = FULL_PIPE;
				return;
			}
			// 0 is acceptable too
			if (client.cgiSentBytes < client.req.content_length)
				client.state = CGI_PIPING;
			else
			{
				close(client.clientPipe[1]);
				client.clientPipe[1] = 0;
				client.state = CGI_FINISHED;
			}
		}
	};

	int	AMethod::_confCheck(Client & client) {

		JSON::Node* var = client.location->search(1, "upload_store");
		if (var == NULL || var->as<std::string const&>() == "")
		{
			client.error(500, true);
			return -1;
		}
		var = client.location->search(1, "cgi");
		if (var == NULL)
		{
			client.error(500, true);
			return -1;
		}
		
		std::string path(var->as<std::string const&>());
		if (*--path.end() == '/')
			path.erase(--path.end());
		std::string filePath = path + client.req.getMethod()[1];
		int	fileAccess = client.fopenr(filePath);
		if (client.fp != NULL)
		{
			fclose(client.fp);
			client.fp = NULL;
		}	
		if (fileAccess == 0)
			return 0;
		else if (fileAccess == 1)
		{
			client.error(403, false);
			return -1;
		}
		else
		{
			client.error(fileAccess, false);
			return -1;
		}
	};
}
