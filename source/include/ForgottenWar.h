#pragma once

/* Standard includes */
#include <exception>
#include <iostream>
#include <map>

/* FW includes */
#include <Common/Types.h>
#include <Logging/Logger.h>
#include <Logging/ConsoleAppender.h>
#include <Logging/FileAppender.h>
#include <Threading/Thread.h>
#include <Threading/LockCriticalSection.h>
#include <Threading/Threadable.h>
#include <Server/SelectServer.h>

enum PlayerStates
{
	Connecting,
	Connected
};

struct PlayerData
{
	fwstr Name;
	sockaddr_in Sockaddr;
	PlayerStates State;
};

class ForgottenWar : public Server::ServerListener
{
public:
	ForgottenWar(fwint Port)
		: server(*this, Port)
	{ }

	ForgottenWar(fwint Port, Logging::Logger &Logger)
		: server(*this, Port, Logger), logger(&Logger)
	{ }

	~ForgottenWar()
	{

	}

	fwvoid Start()
	{
		this->log(Logging::LogLevel::LOG_DEBUG, "Starting server.");

		this->server.Initialize();

		auto st = Threading::Thread(this->server);
		st.Start();

		std::cin.get();

		st.Terminate();

		return;
	}

	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;
		ss << "New client connected from: " << inet_ntoa(Address.sin_addr);

		this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());

		this->addPlayer(ID, Address);

		return;
	}

	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message)
	{
		std::stringstream ss;
		ss << "Received message from user: " << Message.Message;

		this->log(Logging::LogLevel::LOG_INFO, ss.str().c_str());

		auto playerIter = this->players.find(ID);

		// TODO: Fix to handle things like newlines/carriage returns properly
		if (playerIter != this->players.end() && Message.NumBytes > 1)
		{
			std::stringstream ss;

			switch ((*playerIter).second.State)
			{
			case Connecting:
				(*playerIter).second.Name = Message.Message;
				(*playerIter).second.State = Connected;

				ss << "\n\nWelcome to the MUD, " << Message.Message << std::endl;
				this->server.Send(ID, ss.str());

				break;
			case Connected:
				this->broadcastMessage((*playerIter).second, Message.Message);

				break;
			default:
				break;
			}
		}

		return;
	}

	virtual fwvoid ClientDisconnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;
		ss << "Client disconnected: " << inet_ntoa(Address.sin_addr);

		this->log(Logging::LogLevel::LOG_WARN, ss.str().c_str());

		return;
	}

protected:
	Logging::Logger *logger;
	Server::SelectServer server;
	std::map<fwuint, PlayerData> players;

	fwvoid log(Logging::LogLevel Level, const fwchar *Message)
	{
		if (this->logger)
		{
			this->logger->Log(Message, Level);
		}

		return;
	}

	fwvoid addPlayer(fwuint ID, sockaddr_in Address)
	{
		this->players.insert(std::pair<fwuint, PlayerData>(ID, PlayerData { "N/A", Address, Connecting }));
		this->server.Send(ID, "Please enter your name: ");

		return;
	}

	fwvoid broadcastMessage(const PlayerData Speaker, fwstr Message)
	{
		for (auto player : this->players)
		{
			std::stringstream ss;
			ss << "[" << Speaker.Name << "] " << Message << std::endl;

			this->server.Send(player.first, ss.str());
		}

		return;
	}
};
