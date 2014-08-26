#pragma once

#include <vector>
#include <Server/dyad.h>

namespace Server
{
	struct Client
	{
		fwstr Username;
		time_t Connected;
		dyad_Stream *Connection;
	};

	typedef std::vector<Client *> ClientList;
	typedef std::vector<Client *>::iterator ClientIterator;
}