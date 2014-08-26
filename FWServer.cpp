#include "FWServer.h"

namespace Server
{
	FWServer::FWServer(int port)
	{
		this->Port = port;
	}

	fwvoid FWServer::Start()
	{
		dyad_init();

		this->Connection = dyad_newStream();

		dyad_addListener(this->Connection, DYAD_EVENT_ACCEPT, OnAccept, NULL);
		dyad_addListener(this->Connection, DYAD_EVENT_DATA, OnReceive, NULL);
		dyad_addListener(this->Connection, DYAD_EVENT_ERROR, OnError, NULL);

		dyad_listen(this->Connection, this->Port);

		this->Worker = new ServerWorker(this->Connection);
		this->Worker->Run();
	}

	fwvoid FWServer::Stop()
	{
		this->Worker->Halt();
	}

	fwvoid FWServer::OnAccept(dyad_Event *e)
	{
		dyad_addListener(e->remote, DYAD_EVENT_DATA, OnReceive, NULL);
		dyad_writef(e->remote, "Welcome to the server.\r\n");
	}

	fwvoid FWServer::OnReceive(dyad_Event *e)
	{
		dyad_write(e->stream, e->data, e->size);
	}

	fwvoid FWServer::OnError(dyad_Event *e)
	{
		dyad_write(e->stream, e->data, e->size);
	}
}