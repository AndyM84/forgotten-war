#include <GameCore.h>

/* Destructor */

GameCore::~GameCore()
{
	for (auto player : this->world.players)
	{
		player.second.reset();
	}

	this->world.players.clear();

	return;
}

/* Libraries::Library methods */

fwbool GameCore::Setup()
{
	this->world.gameState = FW::GAME_STATES::FWGAME_STARTING;

	// Add our commands to our temporary 'parser'
	this->commands.insert(commandPair("ooc", new Commands::CommOoc));
	this->commands.insert(commandPair("who", new Commands::InfoWho));
	this->commands.insert(commandPair("quit", new Commands::ActionQuit));
	this->commands.insert(commandPair("hotboot", new Commands::AdminHotboot));
	this->commands.insert(commandPair("shutdown", new Commands::AdminShutdown));

	return true;
}

fwbool GameCore::Destroy()
{
	for (auto cmd : this->commands)
	{
		delete cmd.second;
	}

	this->commands.clear();

	return true;
}

/* Libraries::GameLibrary methods */

FW::GAME_STATES GameCore::GameLoop(fwfloat Delta)
{
	if (this->world.gameState == FW::GAME_STATES::FWGAME_STARTING)
	{
		this->world.gameState = FW::GAME_STATES::FWGAME_RUNNING;
	}

	// go through all the players
	for (auto p : this->world.players)
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
				this->BroadcastToAllButPlayer(player->GetClient(), ss.str());

				ss = std::stringstream("");
				ss << "\n\nThanks for playing, " << player->GetName();
				this->SendToClient(player->GetClient(), ss.str());

				continue;
			}

			// our 'parser'
			auto cmdIter = this->commands.find(cmd);

			if (cmdIter != this->commands.end())
			{
				this->world.gameState = cmdIter->second->Process(*this, this->world, buf, player);
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

	return this->world.gameState;
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
		this->world.players.insert(std::pair<fwuint, std::shared_ptr<Player>>(client.plyrid, plyr));
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

	for (auto player : this->world.players)
	{
		if (player.first >= nId)
		{
			nId = player.first + 1;
		}
	}

	auto plyr = std::make_shared<Player>(nId, ID, Address, PLAYER_CONNECTING);
	this->world.players.insert(std::pair<fwuint, std::shared_ptr<Player>>(nId, plyr));
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
	auto playerIter = this->world.players.find(player->GetID());

	if (playerIter != this->world.players.end())
	{
		auto id = player->GetID();
		player.reset();

		this->playerLock.Block();
		this->world.players.erase(playerIter);
		this->playerLock.Release();

		return fwclient { ID, id, Address, CCLIENT_DISCONNECTED };
	}

	return fwclient { ID, 0, NULL, CCLIENT_INVALID };
}

/* GameCore methods */

fwvoid GameCore::Log(const Logging::LogLevel Level, const fwstr Message)
{
	if (this->arbiter)
	{
		this->arbiter->SendLog(Level, Message.c_str());
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
		this->arbiter->SendToClient(Client.sockfd, this->doColor(tmp));
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

fwvoid GameCore::BroadcastToAllButPlayer(const fwclient Client, const fwstr Message)
{
	if (Message.empty())
	{
		return;
	}

	for (auto player : this->world.players)
	{
		if (player.first != Client.plyrid)
		{
			this->SendToClient(player.second->GetClient(), Message);
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

	for (auto player : this->world.players)
	{
		this->SendToClient(player.second->GetClient(), Message);
	}

	return;
}

std::shared_ptr<Player> GameCore::GetPlayerBySocket(fwuint SockFD)
{
	for (auto player : this->world.players)
	{
		if (player.second->GetClient().sockfd == SockFD)
		{
			return player.second;
		}
	}

	return nullptr;
}

fwstr GameCore::doColor(const fwstr original)
{
	if (original.length() < 1)
	{
		return original;
	}

	fwstr result;

	for (fwuint i = 0; i < original.length(); ++i)
	{
		if (original[i] != '`')
		{
			result += original[i];

			continue;
		}

		if (original[i + 1] == '`')
		{
			result += '`';

			continue;
		}

		result += "\u001b";

		switch (original[++i])
		{
		case '0': // [0m
			result += "[0m";
			break;
		case 'w': // [0;37m
			result += "[0;37m";
			break;
		case 'W': // [1;37m
			result += "[1;37m";
			break;
		case 'g': // [0;32m
			result += "[0;32m";
			break;
		case 'G': // [1;32m
			result += "[1;32m";
			break;
		case 'b': // [0;34m
			result += "[0;34m";
			break;
		case 'B': // [1;34m
			result += "[1;34m";
			break;
		case 'r': // [0;31m
			result += "[0;31m";
			break;
		case 'R': // [1;31m
			result += "[1;31m";
			break;
		case 'c': // [0;36m
			result += "[0;36m";
			break;
		case 'C': // [1;36m
			result += "[1;36m";
			break;
		case 'y': // [0;33m
			result += "[0;33m";
			break;
		case 'Y': // [1;33m
			result += "[1;33m";
			break;
		case 'm': // [0;35m
			result += "[0;35m";
			break;
		case 'M': // [1;35m
			result += "[1;35m";
			break;
		case 'k': // [0;30m
			result += "[0;30m";
			break;
		case 'K': // [1;30m
			result += "[1;30m";
			break;
		default:
			++i;
			break;
		}
	}

	result += "\u001b[0m";

	return result;
}

FW_INIT_LIBRARY(GameCore);
