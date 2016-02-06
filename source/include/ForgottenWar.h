#pragma once

/* Standard includes */
#include <algorithm>
#include <exception>
#include <iostream>
#include <map>

/* FW includes */
#include <CommonCore.h>
#include <ServerCore.h>
#include <GameCore.h>

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

		this->server.Stop();
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
		auto msg = this->cleanString(Message.Message);

		if (playerIter != this->players.end() && msg.length() > 0)
		{
			std::stringstream ss;
			auto cmd = this->getCommand(msg);

			switch ((*playerIter).second.State)
			{
			case Connecting:
				(*playerIter).second.Name = msg;
				(*playerIter).second.State = Connected;

				ss << "\n\nWelcome to the MUD, " << msg << std::endl;
				this->server.Send(ID, ss.str());

				ss.clear();
				ss << msg << " has joined the game!" << std::endl;

				this->broadcastMessageToOthers((*playerIter).first, ss.str());

				break;
			case Connected:
				if (msg == "who")
				{
					this->showWho((*playerIter).first);
				}
				else if (cmd == "ooc")
				{
					this->showChat((*playerIter).second, msg.substr(4));
				}
				else if (msg == "quit")
				{
					this->doQuit((*playerIter).first, (*playerIter).second);
				}

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

		auto playerIter = this->players.find(ID);

		if (playerIter != this->players.end() && (*playerIter).second.State == Connected)
		{
			this->doVoid((*playerIter).first, (*playerIter).second);
		}

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

	fwstr getCommand(fwstr Message)
	{
		fwint pos = Message.find(' ');

		if (pos == (fwint)std::string::npos)
		{
			return "";
		}

		return Message.substr(0, pos);
	}

	fwvoid showWho(fwuint ID)
	{
		std::stringstream ss;
		ss << "\nWho's Online\n------------\n";

		for (auto player : this->players)
		{
			ss << player.second.Name;

			if (player.first == ID)
			{
				ss << " (You)";
			}

			ss << "\n";
		}

		this->server.Send(ID, ss.str());

		return;
	}

	fwvoid showChat(const PlayerData Speaker, fwstr Message)
	{
		std::stringstream ss;
		ss << "[OOC] " << Speaker.Name << ": " << Message << "\n";

		this->broadcastMessage(ss.str());

		return;
	}

	fwvoid doQuit(fwuint ID, const PlayerData Player)
	{
		std::stringstream ss;
		ss << Player.Name << " has left the game!\n";
		this->broadcastMessageToOthers(ID, ss.str());

		this->server.Send(ID, "Thanks for playing!\n");
		this->players.erase(ID);
		this->server.Close(ID);

		return;
	}

	fwvoid doVoid(fwuint ID, const PlayerData Player)
	{
		std::stringstream ss;
		ss << Player.Name << " has left the game!\n";
		this->broadcastMessageToOthers(ID, ss.str());

		this->players.erase(ID);

		return;
	}

	fwvoid broadcastMessage(fwstr Message)
	{
		for (auto player : this->players)
		{
			this->server.Send(player.first, Message.c_str());
		}

		return;
	}

	fwvoid broadcastMessageToOthers(fwuint ID, fwstr Message)
	{
		for (auto player : this->players)
		{
			if (player.first != ID)
			{
				this->server.Send(player.first, Message.c_str());
			}
		}

		return;
	}

	fwstr cleanString(fwstr Input)
	{
		for (fwint p = Input.find('\n'); p != (fwint)std::string::npos; p = Input.find('\n'))
		{
			Input.erase(p, 1);
		}

		for (fwint p = Input.find('\r'); p != (fwint)std::string::npos; p = Input.find('\r'))
		{
			Input.erase(p, 1);
		}

		return Input;
	}
};
