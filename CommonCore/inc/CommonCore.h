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

/* Mapping between game player and socket descriptor */
enum ConnectedClientStates
{
	CCLIENT_INVALID,
	CCLIENT_CONNECTING,
	CCLIENT_CONNECTED
};

struct fwclient
{
	fwuint sockfd;
	fwuint plyrid;
	sockaddr_in addr;
	ConnectedClientStates state;
};

// This is an awful hack to allow FW and GameCore to communicate without a proper
// circular reference.  I disgust myself sometimes
class FWSender
{
public:
	virtual fwvoid sendToClient(fwuint ID, fwstr Message) = 0;
};

typedef fwvoid (FWSender::*FWSendMethod)(fwuint, fwstr);

/* A custom type */
// NOTE!
//   So this whole dynamic library thing...it's not so great when you get
//   down to it.  In the future, we'll avoid these things whenever
//   possible, but for now we have a somewhat decent example.
namespace Libraries
{
	class GameLibrary : public Library, public Threading::Threadable
	{
	public:
		virtual fwvoid Run() = 0;
		virtual fwvoid SaveState() = 0;
		virtual fwclient *RestoreState() = 0;
		virtual fwbool ClientIsAdmin(fwuint ID) = 0;
		virtual fwvoid AddCallbacks(const FWSendMethod &send) = 0;
		virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address) = 0;
		virtual fwclient ClientReceived(fwuint ID, const fwstr Message) = 0;
		virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address) = 0;

	protected:
		const FWSendMethod *send;
	};
}
