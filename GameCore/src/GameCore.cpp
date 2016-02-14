#include <GameCore.h>

GameCore::~GameCore()
{
	this->Logger.reset();
	this->gameThread.reset();

	return;
}

fwvoid GameCore::Run()
{
	while (this->gameRunning)
	{
		this->playerLock.Block();

		// process dem peeps
		for (auto p : this->players)
		{
			auto buf = p.second->GetNextMessage();

			if (buf)
			{
				std::stringstream ss;
				ss << "You send message: " << buf->GetRaw();
				this->SendToClient(p.second->GetClient(), ss.str());
			}
		}

		this->playerLock.Release();

		this->Millisleep(30);
	}

	this->log(Logging::LogLevel::LOG_DEBUG, "GameCore - We have run our course");

	return;
}

fwbool GameCore::Setup()
{
	return true;
}

fwbool GameCore::Setup(const Logging::Logger &Logger)
{
	this->Logger = std::make_shared<Logging::Logger>(Logger);
	this->log(Logging::LogLevel::LOG_DEBUG, "GameCore - Logger setup, ready to run");

	return true;
}

fwbool GameCore::Destroy()
{
	this->playerLock.Block();
	this->gameRunning = false;
	this->playerLock.Release();

	if (this->gameThread)
	{
		this->gameThread->Terminate();
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

	this->gameThread = std::make_shared<Threading::Thread>(*this);
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
	this->playerLock.Block();

	fwuint nId = 0;

	for (auto player : this->players)
	{
		if (player.second->GetID() >= nId)
		{
			nId = player.second->GetID() + 1;
		}
	}

	auto plyr = std::make_shared<Player>(nId, ID, Address, CCLIENT_CONNECTED);
	this->players.insert(std::pair<const fwuint, std::shared_ptr<Player>>(nId, plyr));

	this->playerLock.Release();

	return plyr->GetClient();
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

	return (player) ? player->GetClient() : fwclient { ID, 0, NULL, CCLIENT_INVALID };
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
	if (this->Logger)
	{
		//this->Logger->Log(Level, Message);
		std::cout << Message << std::endl;
	}

	return;
}

FW_INIT_LIBRARY(GameCore);
