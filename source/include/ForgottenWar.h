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

class ForgottenWar : public Server::ServerListener, public FWSender
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
		if (this->librarian != NULL)
		{
			delete this->librarian;
		}

		return;
	}

	fwvoid Start()
	{
		this->log(Logging::LogLevel::LOG_DEBUG, "Starting server.");

		this->server.Initialize();

		auto st = Threading::Thread(this->server);
		st.Start();

		this->librarian = new Libraries::Librarian<Libraries::GameLibrary>();

		if (this->logger)
		{
			this->librarian->SetLogger(*this->logger);
		}

		this->game = this->librarian->Load(GAME_CORE);

		if (this->game)
		{
			this->game->Setup();
			this->game->AddCallbacks(*this);
		}

		std::cin.get();

		this->game->Destroy();
		this->librarian->Unload(GAME_CORE);
		this->server.Stop();
		st.Terminate();

		return;
	}

	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;

		if (this->game != NULL)
		{
			auto gId = this->game->ClientConnected(ID, Address);
			this->clients.insert(std::pair<fwuint, fwclient>(ID, gId));

			ss.clear();
			ss << "ForgottenWar - Game returned the following ID for user fd #" << ID << " (" << inet_ntoa(Address.sin_addr) << "): " << gId.plyrid;
			this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());
		}
		else
		{
			ss << "ForgottenWar - New client connected from: " << inet_ntoa(Address.sin_addr);
			this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());
		}

		return;
	}

	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message)
	{
		std::stringstream ss;

		auto clientIter = this->clients.find(ID);
		auto msg = ServerMessage();
		msg.Initialize(Message.Message);

		if (clientIter != this->clients.end() && msg.IsValid())
		{
			if (msg.GetCmd() == "hotboot")
			{				
				if (this->game != NULL)
				{
					this->broadcastMessage("One moment while we change the server.\n\n");

					this->game->SaveState();
					this->librarian->Unload(GAME_CORE);
					this->game = this->librarian->Load(GAME_CORE);
					this->game->Setup();
					this->game->AddCallbacks(*this);

					std::vector<fwclient> ccopy;

					if (!this->clients.empty())
					{
						for (auto client : this->clients)
						{
							ccopy.push_back(client.second);
						}

						this->game->RestoreState(ccopy);
					}

					this->broadcastMessage("Thank you for flying FW Air, we hope you enjoyed the turbulence.\n\n");

					return;
				}
			}
		}

		if (this->game != NULL)
		{
			this->game->ClientReceived(ID, msg);
		}
		else
		{
			ss << "ForgottenWar - Received message from user: " << Message.Message;
			this->log(Logging::LogLevel::LOG_INFO, ss.str().c_str());
		}

		return;
	}

	virtual fwvoid ClientDisconnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;

		if (this->game != NULL)
		{
			this->game->ClientDisconnected(ID, Address);
		}
		else
		{
			ss << "ForgottenWar - Client disconnected: " << inet_ntoa(Address.sin_addr);
			this->log(Logging::LogLevel::LOG_WARN, ss.str().c_str());
		}

		return;
	}

	virtual fwvoid sendToClient(fwuint ID, fwstr Message)
	{
		auto client = this->clients.find(ID);

		if (client != this->clients.end())
		{
			this->server.Send(ID, Message);
		}

		return;
	}

protected:
	Logging::Logger *logger;
	Server::SelectServer server;
	Libraries::GameLibrary *game;
	std::map<fwuint, fwclient> clients;
	Libraries::Librarian<Libraries::GameLibrary> *librarian;

	fwvoid log(Logging::LogLevel Level, const fwchar *Message)
	{
		if (this->logger)
		{
			this->logger->Log(Message, Level);
		}

		return;
	}

	fwvoid broadcastMessage(fwstr Message)
	{
		for (auto player : this->clients)
		{
			this->server.Send(player.first, Message.c_str());
		}

		return;
	}

	fwvoid broadcastMessageToOthers(fwuint ID, fwstr Message)
	{
		for (auto player : this->clients)
		{
			if (player.first != ID)
			{
				this->server.Send(player.first, Message.c_str());
			}
		}

		return;
	}
};
