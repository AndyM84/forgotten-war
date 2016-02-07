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
		virtual fwuint ClientConnected(fwuint ID, const sockaddr_in Address) = 0;
		virtual fwvoid ClientReceived(fwuint ID, const fwstr Message) = 0;
		virtual fwuint ClientDisconnected(fwuint ID, const sockaddr_in Address) = 0;
	};
}
