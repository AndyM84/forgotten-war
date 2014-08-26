#pragma once

#include <Common/Types.h>
#include "ServerWorker.h"
#include <vector>

namespace Server
{
	class FWServer
	{
	public:
		FWServer(int port);
		~FWServer();

		fwvoid Start();
		fwvoid Stop();

		static fwvoid OnAccept(dyad_Event *e);
		static fwvoid OnReceive(dyad_Event *e);
		static fwvoid OnError(dyad_Event *e);

	protected:
		int Port;
		dyad_Stream *Connection;
		ClientList Connections;
		ServerWorker *Worker;
	};
};