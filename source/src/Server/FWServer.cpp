#include <Server/FWServer.h>

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

		dyad_setUpdateTimeout(0.5);

		dyad_addListener(this->Connection, DYAD_EVENT_ACCEPT, OnAccept, NULL);
		dyad_addListener(this->Connection, DYAD_EVENT_DATA, OnReceive, NULL);
		dyad_addListener(this->Connection, DYAD_EVENT_ERROR, OnError, NULL);

		std::cout << "Listening on port " << this->Port << std::endl;

		dyad_listenEx(this->Connection, "127.0.0.1", this->Port, 0);
	}

	fwvoid FWServer::Update()
	{
		dyad_update();
	}

	fwvoid FWServer::Stop()
	{
		dyad_close(this->Connection);
	}

	fwbool FWServer::IsListening()
	{
		return dyad_getStreamCount() > 0;
	}

	fwvoid FWServer::OnAccept(dyad_Event *e)
	{
		std::cout << "New Connection" << std::endl;

		dyad_addListener(e->remote, DYAD_EVENT_DATA, OnReceive, NULL);
		dyad_writef(e->remote, "Welcome to the server.\r\n");
	}

	fwvoid FWServer::OnReceive(dyad_Event *e)
	{
		std::cout << e->data << std::endl;

		dyad_write(e->stream, e->data, e->size);
	}

	fwvoid FWServer::OnError(dyad_Event *e)
	{
		std::cout << "There was an error \"" << e->msg << "\"" << std::endl;
	}
}