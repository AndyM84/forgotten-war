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
		this->log(Logging::LogLevel::LOG_DEBUG, "ForgottenWar - Loading GameCore library");

		this->librarian = new Libraries::Librarian<Libraries::GameLibrary>();

		if (this->logger)
		{
			this->librarian->SetLogger(*this->logger);
		}

		this->game = this->librarian->Load(GAME_CORE);

		this->log(Logging::LogLevel::LOG_DEBUG, "ForgottenWar - Starting server");
		this->server.Initialize();

		this->serverThread = std::make_shared<Threading::Thread>(Threading::Thread(this->server));
		this->serverThread->Start();

		if (this->game)
		{
			this->game->Setup(*this->logger);
			this->game->AddCallbacks(*this);
			this->game->GameStart();
		}

		std::cin.get();

		if (this->game)
		{
			this->librarian->Unload(GAME_CORE);
		}

		this->server.Stop();
		this->serverThread->Terminate();

		return;
	}

	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;

		if (this->game != NULL)
		{
			auto gId = this->game->ClientConnected(ID, Address);
			this->clients.insert(std::pair<fwuint, fwclient>(ID, fwclient(gId)));

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
					ss.clear();
					ss << "ForgottenWar - Hotboot requested by #" << (*clientIter).second.sockfd << " (" << inet_ntoa((*clientIter).second.addr.sin_addr) << ")";
					this->log(Logging::LogLevel::LOG_DEBUG, ss.str().c_str());

					this->broadcastMessage("One moment while we change the server.\n\n");

					this->game->SaveState();
					this->librarian->Unload(GAME_CORE);
				}

				this->game = this->librarian->Load(GAME_CORE);
				this->game->Setup(*this->logger);
				this->game->AddCallbacks(*this);

				if (!this->clients.empty())
				{
					this->log(Logging::LogLevel::LOG_DEBUG, "ForgottenWar - Found orphaned clients during hotboot, restoring GameCore state");

					std::vector<fwclient> ccopy;

					for (auto c : this->clients)
					{
						ccopy.push_back(c.second);
					}

					this->game->RestoreState(ccopy);
				}

				this->log(Logging::LogLevel::LOG_DEBUG, "ForgottenWar - Starting the wee babby GameCore");
				this->game->GameStart();

				this->log(Logging::LogLevel::LOG_DEBUG, "ForgottenWar - Hotboot completed successfully");
				this->broadcastMessage("Thank you for flying FW Air, we hope you enjoyed the turbulence.\n\n");

				return;
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
	Libraries::Librarian<Libraries::GameLibrary> *librarian;
	std::shared_ptr<Threading::Thread> serverThread;
	std::map<fwuint, fwclient> clients;
	Libraries::GameLibrary *game;
	Server::SelectServer server;
	Logging::Logger *logger;

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
