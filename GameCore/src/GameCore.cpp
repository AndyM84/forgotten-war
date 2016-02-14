#include <GameCore.h>

GameCore::~GameCore()
{
	delete this->gameThread;
	this->gameThread = NULL;

	return;
}

fwvoid GameCore::Tick()
{
	// process dem peeps
	for (auto p : this->players)
	{
		if (p.second->GetState() == PLAYER_DISCONNECTED || p.second->GetState() == PLAYER_INVALID)
		{
			continue;
		}

		this->playerLock.Block();
		auto buf = p.second->GetNextMessage();
		this->playerLock.Release();

		if (buf)
		{
			std::stringstream ss;

			ss << "GameCore - Processing message from #" << p.first << ": " << buf->GetRaw();
			this->log(Logging::LogLevel::LOG_DEBUG, ss.str().c_str());

			auto cmd = buf->GetCmd();

			// Assign their name if it's needed
			if (p.second->GetState() == PLAYER_AWAITINGNAME)
			{
				auto toks = buf->GetTokens();

				p.second->SetName(toks[0]);
				p.second->SetState(PLAYER_CONNECTED);

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
				}

				ss << "\n\n";

				this->SendToClient(p.second->GetClient(), ss.str());
			}
			// doOoc
			else if (cmd == "ooc")
			{
				ss = std::stringstream("");
				ss << "[OOC] " << p.second->GetName() << ": " << buf->GetSansCmd() << "\n\n";

				this->BroadcastToAll(ss.str());
			}
			// doQuit
			else if (cmd == "quit")
			{
				ss = std::stringstream("");
				ss << p.second->GetName() << " has left the game!\n";
				this->BroadcastToAllButPlayer(p.second, ss.str());

				this->SendToClient(p.second->GetClient(), "Thanks for playing!\n");
				p.second->SetState(PLAYER_DISCONNECTED);
				this->CloseClient(p.second->GetClient());
			}
		}
		else
		{
			if (p.second->GetState() == PLAYER_CONNECTING)
			{
				this->SendToClient(p.second->GetClient(), "Please enter your name: ");
				p.second->SetState(PLAYER_AWAITINGNAME);
			}
		}
	}

	this->Millisleep(30);

	return;
}

fwbool GameCore::Setup()
{
	return true;
}

fwbool GameCore::Destroy()
{
	this->gameRunning = false;

	if (this->gameThread)
	{
		while (this->isRunning)
		{
			this->Sleep(1);
		}

		this->gameThread->CloseThread();

		delete this->gameThread;
		this->gameThread = NULL;
	}

	this->sender = NULL;
	this->players.clear();

	this->log(Logging::LogLevel::LOG_DEBUG, "GameCore - I have been destroyed!");

	return true;
}

fwvoid GameCore::GameStart()
{
	this->gameRunning = true;

	this->gameThread = new Threading::Thread(*this);
	this->gameThread->Start();

	this->log(Logging::LogLevel::LOG_DEBUG, "GameCore - I have been started!");

	return;
}

fwvoid GameCore::SaveState()
{
	return;
}

fwvoid GameCore::RestoreState(std::vector<fwclient> clients)
{
	for (auto c : clients)
	{
		auto plyr = std::make_shared<Player>(c.plyrid, c.sockfd, c.addr, c.state);
		this->players.insert(std::pair<fwuint, std::shared_ptr<Player>>(c.plyrid, plyr));
	}

	return;
}

fwbool GameCore::ClientIsAdmin(fwuint ID)
{
	return true;
}

fwvoid GameCore::AddCallbacks(FWSender &send)
{
	this->sender = &send;

	return;
}

fwclient GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	fwuint nId = 0;

	for (auto player : this->players)
	{
		if (player.second->GetID() >= nId)
		{
			nId = player.second->GetID() + 1;
		}
	}

	this->playerLock.Block();

	auto plyr = std::make_shared<Player>(nId, ID, Address, CCLIENT_CONNECTING);
	this->players.insert(std::pair<const fwuint, std::shared_ptr<Player>>(nId, plyr));

	this->playerLock.Release();

	return fwclient(plyr->GetClient());
}

fwclient GameCore::ClientReceived(fwuint ID, ServerMessage Message)
{
	if (!Message.IsValid())
	{
		return fwclient{ ID, 0, NULL, CCLIENT_INVALID };
	}

	this->playerLock.Block();
	auto player = this->GetPlayer(ID);

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
	auto iter = this->players.find(ID);

	if (iter != this->players.end())
	{
		this->playerLock.Block();

		fwuint plyrid = (*iter).second->GetID();
		this->players.erase(iter);

		this->playerLock.Release();

		return fwclient { ID, plyrid, Address, CCLIENT_DISCONNECTED };
	}

	return fwclient { ID, 0, NULL, CCLIENT_INVALID };
}

fwvoid GameCore::SendToClient(const fwclient Client, const fwstr Message) const
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

	if (Message[Message.length() - 1] != ' ' && Message[Message.length() - 1] != '\n' && Message[Message.length() - 2] != '\r')
	{
		tmp += ' ';
	}

	this->sender->sendToClient(Client.sockfd, tmp);

	return;
}

fwvoid GameCore::CloseClient(const fwclient Client) const
{
	this->sender->closeClient(Client.sockfd);

	return;
}

fwvoid GameCore::BroadcastToAllButPlayer(const std::shared_ptr<Player> Client, const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	for (auto p : this->players)
	{
		if (p.second->GetID() != Client->GetID())
		{
			this->SendToClient(p.second->GetClient(), Message);
		}
	}

	return;
}

fwvoid GameCore::BroadcastToAll(const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	for (auto p : this->players)
	{
		this->SendToClient(p.second->GetClient(), Message);
	}

	return;
}

const std::shared_ptr<Player> GameCore::GetPlayer(fwuint ID) const
{
	auto client = this->players.find(ID);

	if (client == this->players.end())
	{
		return NULL;
	}

	return (*client).second;
}

const std::vector<fwclient> GameCore::GetClients() const
{
	std::vector<fwclient> tmp;

	for (auto client : this->players)
	{
		tmp.push_back(client.second->GetClient());
	}

	return tmp;
}

fwvoid GameCore::log(const Logging::LogLevel Level, const fwchar *Message)
{
	if (this->sender)
	{
		this->sender->sendLog(Level, Message);
	}

	return;
}

FW_INIT_LIBRARY(GameCore);
