#include <GameCore.h>

/* Destructor */

GameCore::~GameCore()
{
	for (auto player : this->players)
	{
		player.second.reset();
	}

	this->players.clear();

	return;
}

/* Libraries::Library methods */

fwbool GameCore::Setup()
{
	this->gameState = FW::GAME_STATES::FWGAME_STARTING;

	return true;
}

fwbool GameCore::Destroy()
{
	return true;
}

/* Libraries::GameLibrary methods */

FW::GAME_STATES GameCore::GameLoop(fwfloat Delta)
{
	if (this->gameState == FW::GAME_STATES::FWGAME_STARTING)
	{
		this->gameState = FW::GAME_STATES::FWGAME_RUNNING;
	}

	// go through all the players
	for (auto p : this->players)
	{
		auto player = p.second;

		if (!player || player->GetState() == PLAYER_DISCONNECTED || player->GetState() == PLAYER_INVALID)
		{
			continue;
		}

		this->playerLock.Block();
		auto buf = player->GetNextMessage();
		this->playerLock.Release();

		if (buf)
		{
			std::stringstream ss;

			ss << "GameCore - Processing message from #" << player->GetID() << ": " << buf->GetRaw();
			this->Log(Logging::LogLevel::LOG_DEBUG, ss.str().c_str());

			auto cmd = buf->GetCmd();

			if (player->GetState() == PLAYER_AWAITINGNAME)
			{
				auto tok = buf->GetTokens();

				player->SetName(tok[0]);
				player->SetState(PLAYER_CONNECTED);

				ss = std::stringstream("");
				ss << player->GetName() << " has connected!";
				this->BroadcastToAllButPlayer(player, ss.str());

				ss = std::stringstream("");
				ss << "\n\nThanks for playing, " << player->GetName();
				this->SendToClient(player->GetClient(), ss.str());

				continue;
			}

			// doWho
			if (cmd == "who")
			{
				ss = std::stringstream("");
				ss << "\nWho's Online\n------------\n";

				for (auto pl : this->players)
				{
					ss << pl.second->GetName();

					if (pl.first == p.first)
					{
						ss << " (You)";
					}

					ss << "\n";
				}

				ss << "\n\n";
				this->SendToClient(player->GetClient(), ss.str());
			}
			// doOoc
			else if (cmd == "ooc")
			{
				ss = std::stringstream("");
				ss << "[OOC] " << player->GetName() << ": " << buf->GetSansCmd() << "\n\n";
				this->BroadcastToAll(ss.str());
			}
			// doQuit
			else if (cmd == "quit")
			{
				ss = std::stringstream("");
				ss << player->GetName() << " has left the game!";
				this->BroadcastToAllButPlayer(player, ss.str());

				this->SendToClient(player->GetClient(), "Thanks for playing, come back soon!\n\n");
				this->CloseClient(player->GetClient());
			}
			else
			{
				this->SendToClient(player->GetClient(), "That is not a known command.");
			}
		}
		else
		{
			if (player->GetState() == PLAYER_CONNECTING)
			{
				this->SendToClient(player->GetClient(), "Please enter your name: ");
				player->SetState(PLAYER_AWAITINGNAME);
			}
		}
	}

	return this->gameState;
}

fwvoid GameCore::SaveState()
{
	return;
}

fwvoid GameCore::RestoreState(std::vector<fwclient> clients)
{
	for (auto client : clients)
	{
		auto plyr = std::make_shared<Player>(client.plyrid, client.sockfd, client.addr, PLAYER_CONNECTING);
		this->players.insert(std::pair<fwuint, std::shared_ptr<Player>>(client.plyrid, plyr));
	}

	return;
}

fwvoid GameCore::AddArbiter(FW::CoreArbiter &send)
{
	this->arbiter = &send;

	return;
}

fwclient GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	fwuint nId = 0;
	this->playerLock.Block();

	for (auto player : this->players)
	{
		if (player.first >= nId)
		{
			nId = player.first + 1;
		}
	}

	auto plyr = std::make_shared<Player>(nId, ID, Address, PLAYER_CONNECTING);
	this->players.insert(std::pair<fwuint, std::shared_ptr<Player>>(nId, plyr));
	this->playerLock.Release();

	this->SendToClient(plyr->GetClient(), "Please enter your name: ");

	return plyr->GetClient();
}

fwclient GameCore::ClientReceived(fwuint ID, ServerMessage Message)
{
	if (!Message.IsValid())
	{
		return fwclient { ID, 0, NULL, CCLIENT_INVALID };
	}

	this->playerLock.Block();
	auto player = this->GetPlayerBySocket(ID);

	if (player)
	{
		auto msg = Message;
		player->AddBufferMessage(std::make_shared<ServerMessage>(msg));
	}

	this->playerLock.Release();

	return (player) ? fwclient(player->GetClient()) : fwclient { ID, 0, NULL, CCLIENT_INVALID };
}

fwclient GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	auto player = this->GetPlayerBySocket(ID);
	auto playerIter = this->players.find(player->GetID());

	if (playerIter != this->players.end())
	{
		auto id = player->GetID();
		player.reset();

		this->playerLock.Block();
		this->players.erase(playerIter);
		this->playerLock.Release();

		return fwclient { ID, id, Address, CCLIENT_DISCONNECTED };
	}

	return fwclient { ID, 0, NULL, CCLIENT_INVALID };
}

/* GameCore methods */

fwvoid GameCore::Log(const Logging::LogLevel Level, const fwchar *Message)
{
	if (this->arbiter)
	{
		this->arbiter->SendLog(Level, Message);
	}

	return;
}

fwvoid GameCore::SendToClient(const fwclient Client, const fwstr Message)
{
	if (Message.empty())
	{
		return;
	}

	fwstr tmp;

	if (Message[0] != '\n' && Message[0] != '\r')
	{
		tmp += "\n\n";
	}

	tmp += Message;

	if (Message[Message.length() - 1] != ' ' && Message[Message.length() - 1] != '\n' && Message[Message.length() - 1] != '\r')
	{
		tmp += ' ';
	}

	if (this->arbiter)
	{
		this->arbiter->SendToClient(Client.sockfd, tmp);
	}

	return;
}

fwvoid GameCore::CloseClient(const fwclient Client)
{
	if (this->arbiter)
	{
		this->arbiter->CloseClient(Client.sockfd);
	}

	return;
}

fwvoid GameCore::BroadcastToAllButPlayer(const std::shared_ptr<Player> Client, const fwstr Message)
{
	if (Message.empty())
	{
		return;
	}

	for (auto player : this->players)
	{
		if (player.first != Client->GetID())
		{
			this->SendToClient(Client->GetClient(), Message);
		}
	}

	return;
}

fwvoid GameCore::BroadcastToAll(const fwstr Message)
{
	if (Message.empty())
	{
		return;
	}

	for (auto player : this->players)
	{
		this->SendToClient(player.second->GetClient(), Message);
	}

	return;
}

std::vector<std::shared_ptr<Player>> GameCore::GetPlayers()
{
	std::vector<std::shared_ptr<Player>> copy;

	for (auto player : this->players)
	{
		copy.push_back(player.second);
	}

	return copy;
}

std::shared_ptr<Player> GameCore::GetPlayer(fwuint ID)
{
	for (auto player : this->players)
	{
		if (player.first == ID)
		{
			return player.second;
		}
	}

	return nullptr;
}

std::shared_ptr<Player> GameCore::GetPlayerBySocket(fwuint SockFD)
{
	for (auto player : this->players)
	{
		if (player.second->GetClient().sockfd == SockFD)
		{
			return player.second;
		}
	}

	return nullptr;
}

FW_INIT_LIBRARY(GameCore);
