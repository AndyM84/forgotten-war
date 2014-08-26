#include <Server/ServerWorker.h>

namespace Server
{
	ServerWorker::ServerWorker(dyad_Stream *connection)
	{
		this->Connection = connection;
	}

	fwvoid ServerWorker::Run()
	{
		while (dyad_getStreamCount() > 0) {
			dyad_update();
		}

		dyad_shutdown();
	}

	ClientList *ServerWorker::GetNewConnections()
	{
		return new ClientList();
	}

	fwvoid ServerWorker::Broadcast(ClientList *connections, fwstr message)
	{

	}
}
