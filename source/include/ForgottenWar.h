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

#define GAME_CORE "GameCore.dll"

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
	fwvoid GameLoop();

protected:
	Libraries::Librarian<Libraries::GameLibrary> *librarian;
	std::shared_ptr<Threading::Thread> serverThread;
	std::shared_ptr<Logging::Logger> logger;
	std::map<fwuint, fwclient> clients;
	Libraries::GameLibrary *game;
	Server::SelectServer *server;

	/* Protected methods */

	/* Logs through Logger if available */
	fwvoid log(Logging::LogLevel Level, const fwchar *Message);
	/* Sends a message to all connected clients */
	fwvoid broadcastMessage(fwstr Message);
	/* Sends a message to all connected clients except the sender */
	fwvoid broadcastMessageToOthers(fwuint ID, fwstr Message);
};
