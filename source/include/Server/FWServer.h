#pragma once

#include <Common/Types.h>
#include <Server/ServerData.h>
#include <vector>

namespace Server
{
	class FWServer
	{
	public:
		FWServer(int port);
		~FWServer();

		fwvoid Start();
		fwbool IsListening();
		fwvoid Update();
		fwvoid Stop();

		static fwvoid OnAccept(dyad_Event *e);
		static fwvoid OnReceive(dyad_Event *e);
		static fwvoid OnError(dyad_Event *e);

	protected:
		fwint Port;
		dyad_Stream *Connection;
		ClientList Connections;
	};
};