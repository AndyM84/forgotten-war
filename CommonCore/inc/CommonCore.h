#pragma once

/* Base types/includes */
#include <Common/Types.h>

/* Logging system */
#include <Logging/ConsoleAppender.h>
#include <Logging/FileAppender.h>
#include <Logging/Logger.h>

/* Threading system */
#include <Threading/LockCriticalSection.h>
#include <Threading/LockMutex.h>
#include <Threading/Thread.h>

/* Library system */
#include <Libraries/Library.h>
#include <Libraries/Librarian.h>

/* 3rd Party Libraries */
#include "../../n2framework/include/N2f.hpp"
#include <jsmn.h>

// Other files
#include <GameConfig.h>
#include <ServerMessage.h>

/* Mapping between game player and socket descriptor */
enum ConnectedClientStates
{
	CCLIENT_INVALID,
	CCLIENT_CONNECTING,
	CCLIENT_CONNECTED,
	CCLIENT_DISCONNECTED
};

struct fwclient
{
	fwuint sockfd;
	fwuint plyrid;
	sockaddr_in addr;
	ConnectedClientStates state;
};

namespace FW
{
	enum GAME_STATES
	{
		FWGAME_INVALID,
		FWGAME_STARTING,
		FWGAME_HOTBOOTING,
		FWGAME_RUNNING,
		FWGAME_STOPPING
	};

	/* To allow the DLL to communicate back */
	class CoreArbiter
	{
	public:
		virtual fwvoid SendToClient(fwuint ID, fwstr Message) = 0;
		virtual fwvoid CloseClient(fwuint ID) = 0;
		virtual fwvoid SendLog(Logging::LogLevel Level, const fwchar *Message) = 0;
	};
}

/* A custom type */
namespace Libraries
{
	class GameLibrary : public Library
	{
	public:
		virtual ~GameLibrary()
		{
			if (this->arbiter)
			{
				this->arbiter = nullptr;
			}

			return;
		}

		virtual fwbool Setup() = 0;
		virtual fwbool Setup(const GameConfig &config) = 0;
		virtual FW::GAME_STATES GameLoop(fwfloat Delta) = 0;
		virtual fwvoid SaveState() = 0;
		virtual fwvoid RestoreState(std::vector<fwclient> clients) = 0;
		virtual fwvoid AddArbiter(FW::CoreArbiter &send) = 0;
		virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address) = 0;
		virtual fwclient ClientReceived(fwuint ID, ServerMessage Message) = 0;
		virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address) = 0;
		virtual fwvoid Log(const Logging::LogLevel Level, const fwstr Message) = 0;
		virtual fwvoid SendToClient(const fwclient Client, const fwstr Message) = 0;
		virtual fwvoid CloseClient(const fwclient Client) = 0;
		virtual fwvoid BroadcastToAllButPlayer(const fwclient Client, const fwstr Message) = 0;
		virtual fwvoid BroadcastToAll(const fwstr Message) = 0;

	protected:
		FW::CoreArbiter *arbiter;
	};
}
