#include <Server/FWServer.h>
#include <ctype.h>

namespace Server
{
	FWServer::FWServer(fwstr host, int port)
	{
		this->Host = host;
		this->Port = port;
		this->Characters = CharList();
	}

	fwvoid FWServer::Start()
	{
		dyad_init();

		this->Connection = dyad_newStream();

		dyad_setUpdateTimeout(0.5);

		dyad_addListener(this->Connection, DYAD_EVENT_ACCEPT, OnAccept, (void *) this);
		dyad_addListener(this->Connection, DYAD_EVENT_DATA, OnReceive, (void *) this);
		dyad_addListener(this->Connection, DYAD_EVENT_ERROR, OnError, (void *) this);

		std::cout << "Listening on port " << this->Port << std::endl;

		dyad_listenEx(this->Connection, this->Host.c_str(), this->Port, 0);
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

	fwvoid FWServer::Broadcast(fwstr message)
	{
		if (!this->Characters.empty())
		{
			for (CharIterator iter = this->Characters.begin(); iter != this->Characters.end(); iter++)
			{
				dyad_writef((*iter)->Stream, message.c_str());
			}
		}
	}

	Character *FWServer::NewCharacter(dyad_Stream *stream)
	{
		Character *player = new Character();
		player->Stream = stream;

		this->Characters.push_back(player);

		return player;
	}

	Character *FWServer::GetCharacter(dyad_Stream *stream)
	{
		if (!this->Characters.empty())
		{
			for (CharIterator iter = this->Characters.begin(); iter != this->Characters.end(); iter++)
			{
				if ((*iter)->Stream == stream)
				{
					return *iter;
				}
			}

			return this->NewCharacter(stream);
		}
		else
		{
			return this->NewCharacter(stream);
		}
	}

	fwvoid FWServer::RemoveCharacter(Character *character)
	{

	}

	fwvoid FWServer::OnAccept(dyad_Event *e)
	{
		FWServer *Server = (FWServer *)e->udata;
		Character *Char = Server->NewCharacter(e->remote);

		Char->Stream = e->remote;
		Char->IsRegistered = false;

		std::cout << "New Connection" << std::endl;

		dyad_writef(Char->Stream, "Welcome to the server.\r\n");
		dyad_writef(Char->Stream, "What is your username?\r\n");

		dyad_addListener(Char->Stream, DYAD_EVENT_DATA, OnReceive, NULL);
	}

	fwvoid FWServer::OnReceive(dyad_Event *e)
	{
		FWServer *Server = (FWServer *)e->udata;
		Character *Char = Server->GetCharacter(e->remote);

		if (Char->IsRegistered)
		{
			fwstr msg = "[" + Char->Username + "]: " + e->data + "\r\n";
			Server->Broadcast(msg);
		}
		else
		{
			Char->IsRegistered = true;

			int i = 0;
			while (e->data[i])
			{
				if (!isalpha(e->data[i]))
				{
					Char->IsRegistered = false;
					dyad_writef(Char->Stream, "What is your username?\r\n");
				}
			}
		}
	}

	fwvoid FWServer::OnError(dyad_Event *e)
	{
		FWServer *Server = (FWServer *)e->udata;

		std::cout << "There was an error \"" << e->msg << "\"" << std::endl;
	}
}