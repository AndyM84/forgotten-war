#include <Server/ServerWorker.h>

namespace Server
{
	ServerWorker::ServerWorker(dyad_Stream *connection)
	{
		this->Connection = connection;
	}

	fwvoid ServerWorker::Run()
	{

	}

	ClientList *ServerWorker::GetNewConnections()
	{
		return new ClientList();
	}

	fwvoid ServerWorker::Broadcast(ClientList *connections, fwstr message)
	{

	}

	fwvoid ServerWorker::Halt()
	{
		dyad_shutdown();
	}
}
