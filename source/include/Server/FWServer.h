#pragma once

#include <Common/Types.h>
#include <Server/ServerData.h>
#include <vector>

namespace Server
{
	class FWServer
	{
	public:
		FWServer(fwstr host, int port);
		~FWServer();

		fwvoid Start();
		fwbool IsListening();
		fwvoid Update();
		fwvoid Stop();

		static fwvoid OnAccept(dyad_Event *e);
		static fwvoid OnReceive(dyad_Event *e);
		static fwvoid OnError(dyad_Event *e);
		static fwvoid OnClientDisconnect(dyad_Event *e);

		Character *NewCharacter(dyad_Stream *stream);
		Character *GetCharacter(dyad_Stream *stream);
		fwvoid RemoveCharacter(Character *character);

		fwvoid Broadcast(dyad_Stream *from, fwstr message);

		CharList Characters;

	protected:
		static FWServer *Instance;
		fwstr Host;
		fwint Port;
		dyad_Stream *Connection;
	};
};