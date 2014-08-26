#pragma once

#include <Threading/Threadable.h>
#include <Server/ServerData.h>
#include <Server/dyad.h>

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