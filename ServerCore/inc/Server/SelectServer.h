#pragma once

#include <Common/Types.h>
#include <Server/ServerListener.h>
#include <Threading/Threadable.h>
#include <Threading/LockCriticalSection.h>
#include <Logging/Logger.h>

#include <memory>
#include <sstream>
#include <vector>

#define MAX_RECV_LENGTH 4096

namespace Server
{
	class SelectServer : public Threading::Threadable
	{
	public:
		SelectServer(ServerListener &Listener, fwint Port);
		SelectServer(ServerListener &Listener, fwint Port, Logging::Logger &Logger);
		~SelectServer();

		fwvoid Initialize();
		fwvoid Send(const fwuint ID, fwstr Message);
		fwvoid Close(const fwuint ID);
		fwvoid Stop();

	protected:
		// External resources
		ServerListener *Listener;
		Logging::Logger *Logger;

		// Internal resources
		std::vector<SocketConn> clients;
		Threading::LockMutex lock;
		fwbool isInitialized;
		SOCKET listenSocket;
		SOCKET clientSocket;
		fwbool shouldRun;
		fd_set setExcept;
		fd_set setWrite;
		fd_set setRead;
		fwint port;

		virtual fwvoid Tick();
		fwvoid initSets();
		fwvoid log(const Logging::LogLevel Level, const fwchar *Message);
		fwint getSocketError(SOCKET socket);
		fwuint addClient(SOCKET socket, sockaddr_in address);
	};
}
