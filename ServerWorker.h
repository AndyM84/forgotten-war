#pragma once

#include <Threading/Threadable.h>
#include "ServerData.h"
#include "dyad.h"

namespace Server
{
	class ServerWorker : public Threading::Threadable
	{
	public:
		ServerWorker(dyad_Stream *connection);
		virtual fwvoid Run();
		fwvoid Halt();
		ClientList *GetNewConnections();
		static fwvoid Broadcast(ClientList *connections, fwstr message);

	protected:
		dyad_Stream *Connection;
	};
}