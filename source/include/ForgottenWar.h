#pragma once

/* Standard includes */
#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <string>

/* FW includes */
#include <CommonCore.h>
#include <ServerCore.h>
#include <CliDispatch.h>

#define GAME_CORE           "GameCore.dll"
#define GAME_EVENT_NAME     TEXT("FWGameEvent")
#define HOTBOOT_PASSWORD    "123456"
#define HOTBOOT_STRT_MSG    "One moment while we change our clothes.\n\n"
#define HOTBOOT_STOP_MSG    "Thank you for flying FW Air, we hope you enjoyed the turbulence.\n\n"

class ForgottenWar : public Server::ServerListener, public FW::CoreArbiter
{
public:
	/* Ctor's and dtor */

	/* Basic ctor */
	ForgottenWar(fwuint Port);
	/* Ctor that allows for shared logging */
	ForgottenWar(fwuint Port, Logging::Logger &Logger);
	/* Dtor */
	~ForgottenWar();

	/* Server::ServerListener methods */

	/* Called when a client connects to the SelectServer */
	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address);
	/* Called when a full message is received from a client with a newline */
	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message);
	/* Called when a client has disconnected */
	virtual fwvoid ClientDisconnected(fwuint ID, const sockaddr_in Address);

	/* FW::CoreArbiter methods */

	/* Public method for GameCore to send messages through */
	virtual fwvoid SendToClient(fwuint ID, fwstr Message);
	/* Public method for GameCore to close client connections */
	virtual fwvoid CloseClient(fwuint ID);
	/* Public method for GameCore to send logs through */
	virtual fwvoid SendLog(Logging::LogLevel Level, const fwchar *Message);

	fwvoid Initialize();
	FW::GAME_STATES GameLoop();
	fwvoid Stop();

protected:
	Libraries::Librarian<Libraries::GameLibrary> librarian;
	std::shared_ptr<Threading::Thread> serverThread;
	Threading::LockCriticalSection gameLock;
	std::map<fwuint, fwclient> clients;
	Server::SelectServer *server;
	Libraries::GameLibrary *game;
	FW::GAME_STATES gameState;
	Logging::Logger *logger;
	fwhandle gameEvent;

	/* Protected methods */

	/* Logs through Logger if available */
	fwvoid log(Logging::LogLevel Level, const fwchar *Message);
	/* Sends a message to all connected clients */
	fwvoid broadcastMessage(fwstr Message);
	/* Sends a message to all connected clients except the sender */
	fwvoid broadcastMessageToOthers(fwuint ID, fwstr Message);
	/* Reloads the GameCore library in a nice orderly fashion */
	fwvoid hotbootCore();
};
