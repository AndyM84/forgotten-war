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

/* N2f */
#include <N2f/N2f.h>

// Server message
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

/* To allow the DLL to communicate back */
class FWSender
{
public:
	virtual fwvoid sendToClient(fwuint ID, fwstr Message) = 0;
	virtual fwvoid closeClient(fwuint ID) = 0;
	virtual fwvoid sendLog(Logging::LogLevel Level, const fwchar *Message) = 0;
};

/* A custom type */
namespace Libraries
{
	class GameLibrary : public Library
	{
	public:
		virtual ~GameLibrary()
		{
			if (this->sender)
			{
				this->sender = NULL;
			}

			return;
		}

		virtual fwvoid GameTick() = 0;
		virtual fwvoid GameStart() = 0;
		virtual fwvoid SaveState() = 0;
		virtual fwvoid RestoreState(std::vector<fwclient> clients) = 0;
		virtual fwbool ClientIsAdmin(fwuint ID) = 0;
		virtual fwvoid AddCallbacks(FWSender &send) = 0;
		virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address) = 0;
		virtual fwclient ClientReceived(fwuint ID, ServerMessage Message) = 0;
		virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address) = 0;

	protected:
		FWSender *sender;
	};
}
