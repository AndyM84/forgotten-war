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
	ForgottenWar(fwuint Port);
	ForgottenWar(fwuint Port, Logging::Logger &Logger);
	~ForgottenWar();

	/* Server::ServerListener methods */
	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message);
	virtual fwvoid ClientDisconnected(fwuint ID, const sockaddr_in Address);

	/* ForgottenWar::CoreArbiter methods */
	virtual fwvoid sendToClient(fwuint ID, fwstr Message);
	virtual fwvoid closeClient(fwuint ID);
	virtual fwvoid sendLog(Logging::LogLevel Level, const fwchar *Message);

protected:
	Libraries::Librarian<Libraries::GameLibrary> *librarian;
	std::shared_ptr<Threading::Thread> serverThread;
	std::map<fwuint, fwclient> clients;
	Libraries::GameLibrary *game;
	Server::SelectServer server;
	Logging::Logger *logger;

	fwvoid log(Logging::LogLevel Level, const fwchar *Message);
	fwvoid broadcastMessage(fwstr Message);
	fwvoid broadcastMessageToOthers(fwuint ID, fwstr Message);
};
