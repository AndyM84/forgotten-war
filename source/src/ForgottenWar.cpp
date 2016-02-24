#include <ForgottenWar.h>

/* Ctor's and dtor */

ForgottenWar::ForgottenWar(fwuint Port)
{
	this->logger = nullptr;
	this->server = new Server::SelectServer(*this, Port, *this->logger);
	this->gameEvent = CreateEvent(NULL, true, false, GAME_EVENT_NAME);
	this->gameState = FW::GAME_STATES::FWGAME_STARTING;

	return;
}

ForgottenWar::ForgottenWar(fwuint Port, Logging::Logger &Logger)
{
	this->logger = &Logger;
	this->server = new Server::SelectServer(*this, Port, *this->logger);
	this->gameEvent = CreateEvent(NULL, true, false, GAME_EVENT_NAME);
	this->gameState = FW::GAME_STATES::FWGAME_STARTING;

	return;
}

ForgottenWar::~ForgottenWar()
{
	this->Stop();

	return;
}

/* Server::ServerListener methods */

fwvoid ForgottenWar::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	std::stringstream ss;

	if (this->game)
	{
		auto gclient = this->game->ClientConnected(ID, Address);
		this->clients.insert(std::pair<fwuint, fwclient>(ID, fwclient(gclient)));

		ss << "FW - Game returned the following ID for fd #" << ID << " (" << inet_ntoa(Address.sin_addr) << "): " << gclient.plyrid;
		this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());
	}
	else
	{
		fwuint nId = 0;

		for (auto c : this->clients)
		{
			if (c.second.plyrid >= nId)
			{
				nId = c.second.plyrid + 1;
			}
		}

		this->clients.insert(std::pair<fwuint, fwclient>(ID, fwclient { ID, nId, Address, CCLIENT_CONNECTING }));

		ss << "FW - Added orphaned new client connected from: " << inet_ntoa(Address.sin_addr);
		this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());
	}

	return;
}

fwvoid ForgottenWar::ClientReceived(fwuint ID, const Server::SocketMessage &Message)
{
	// TODO: Add a config directive that the server can use to know a 'core' hotboot password

	std::stringstream ss;
	auto clientIter = this->clients.find(ID);

	auto msg = ServerMessage();
	msg.Initialize(Message.Message);

	if (clientIter != this->clients.end() && msg.IsValid())
	{
		auto cmd = msg.GetCmd();
		auto tok = msg.GetTokens();

		if (cmd == "hotboot" && tok.size() == 2 && tok[1] == HOTBOOT_PASSWORD)
		{
			this->gameState = FW::GAME_STATES::FWGAME_HOTBOOTING;
			SetEvent(this->gameEvent);
		}
		else
		{
			auto cl = this->game->ClientReceived(ID, msg);
			(*clientIter).second.state = cl.state;
		}
	}
	else if (clientIter == this->clients.end())
	{
		ss = std::stringstream("");
		ss << "FW - Received input from invalid user #" << ID << ": " << msg.GetRaw();
		this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());
	}
	else if (!msg.IsValid())
	{
		ss = std::stringstream("");
		ss << "FW - Received invalid input from user #" << ID << ": " << msg.GetRaw();
		this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());
	}

	return;
}

fwvoid ForgottenWar::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	std::stringstream ss;
	auto clientIter = this->clients.find(ID);

	if (clientIter != this->clients.end())
	{
		if (this->game)
		{
			auto cl = this->game->ClientDisconnected(ID, Address);

			if (cl.state == CCLIENT_DISCONNECTED || cl.state == CCLIENT_INVALID)
			{
				this->clients.erase(clientIter);
			}
			else
			{
				ss << "FW - GameCore did not disconnect client #" << ID;
				this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());
			}
		}
		else
		{
			ss << "FW - Client #" << ID << " has disconnected";
			this->log(Logging::LogLevel::LOG_DEBUG, ss.str().c_str());

			this->clients.erase(clientIter);
		}
	}
	else
	{
		ss << "FW - Received disconnect for non-existent client #" << ID;
		this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());
	}

	return;
}

/* ForgottenWar::CoreArbiter methods */

fwvoid ForgottenWar::SendToClient(fwuint ID, fwstr Message)
{
	auto client = this->clients.find(ID);

	if (client != this->clients.end() && this->server)
	{
		this->server->Send(ID, Message);
	}

	return;
}

fwvoid ForgottenWar::CloseClient(fwuint ID)
{
	auto client = this->clients.find(ID);

	if (client != this->clients.end() && this->server)
	{
		this->server->Close(ID);
	}

	return;
}

fwvoid ForgottenWar::SendLog(Logging::LogLevel Level, const fwchar *Message)
{
	this->log(Level, Message);

	return;
}

fwvoid ForgottenWar::Initialize()
{
	// Assign the logger to the librarian if we've got it
	if (this->logger)
	{
		this->librarian.SetLogger(*this->logger);
	}

	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Loading GameCore library to start game");
	this->game = this->librarian.Load(GAME_CORE);

	if (!this->game)
	{
		this->log(Logging::LogLevel::LOG_CRITICAL, "FW - GameCore was unable to load, abort abort abort");

		return;
	}

	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Starting the SelectServer");
	this->server->Initialize();

	this->serverThread = std::make_shared<Threading::Thread>(Threading::Thread(*this->server));
	this->serverThread->Start();

	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Setting up the GameCore instance");

	if (this->game)
	{
		this->game->AddArbiter(*this);
		this->game->Setup();
	}

	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Game has been started, proceed to loop and collect $200");
	this->gameState = FW::GAME_STATES::FWGAME_RUNNING;

	return;
}

FW::GAME_STATES ForgottenWar::GameLoop()
{
	// Check if we need to exit
	this->gameLock.Block();

	if (this->gameState == FW::GAME_STATES::FWGAME_STOPPING)
	{
		this->gameLock.Release();

		// if we're here, we are done-done
		if (this->game)
		{
			this->game = nullptr;
			this->librarian.Unload(GAME_CORE);
		}

		this->server->Stop();
		this->serverThread->Terminate();
		this->serverThread->CloseThread();

		this->log(Logging::LogLevel::LOG_DEBUG, "FW - The game has shut down, congratumalations");

		return FW::GAME_STATES::FWGAME_INVALID;
	}

	this->gameLock.Release();

	// TODO: Add a delta timing...thing

	// do loop here
	auto coreState = this->game->GameLoop(0.0);

	// so if we're asked to hotBoot, just do it and restart the loop
	if (coreState == FW::GAME_STATES::FWGAME_HOTBOOTING)
	{
		this->hotbootCore();

		return this->gameState;
	}
	// but if we're being asked to stop, copy state and restart loop
	else if (coreState == FW::GAME_STATES::FWGAME_STOPPING)
	{
		this->gameLock.Block();
		this->gameState = coreState;
		this->gameLock.Release();

		return this->gameState;
	}

	// If we intercepted an UBER IMPORTANT SPECIAL hotboot cmd
	if (WaitForSingleObject(this->gameEvent, 5) == WAIT_TIMEOUT)
	{
		return this->gameState;
	}

	// if we got here, we were TRIGGERED!
	if (this->gameState == FW::GAME_STATES::FWGAME_HOTBOOTING)
	{
		this->hotbootCore();

		return this->gameState;
	}

	return this->gameState;
}

fwvoid ForgottenWar::Stop()
{
	if (this->game)
	{
		this->game = NULL;
		this->librarian.Unload(GAME_CORE);
	}

	if (this->server && this->serverThread)
	{
		// TODO: This should have a wait-able thing in it for the server to say it's shut down
		this->serverThread->Terminate();

		this->serverThread.reset();
		delete this->server;
	}

	this->logger = nullptr;

	return;
}

/* Protected methods */

fwvoid ForgottenWar::log(Logging::LogLevel Level, const fwchar *Message)
{
	if (this->logger)
	{
		this->logger->Log(Message, Level);
	}

	return;
}

fwvoid ForgottenWar::broadcastMessage(fwstr Message)
{
	if (this->server)
	{
		for (auto client : this->clients)
		{
			this->server->Send(client.first, Message);
		}
	}

	return;
}

fwvoid ForgottenWar::broadcastMessageToOthers(fwuint ID, fwstr Message)
{
	if (this->server)
	{
		for (auto client : this->clients)
		{
			if (client.first != ID)
			{
				this->server->Send(client.first, Message);
			}
		}
	}

	return;
}

fwvoid ForgottenWar::hotbootCore()
{
	// Ok we're here, let's just assume we were signaled
	ResetEvent(this->gameEvent);

	// HI MOM!
	this->broadcastMessage(HOTBOOT_STRT_MSG);

	// first clear the library if it's there (it may not be if there was a problem)
	if (this->game)
	{
		this->game->SaveState();
		this->game = nullptr;

		this->librarian.Unload(GAME_CORE);

		this->log(Logging::LogLevel::LOG_DEBUG, "FW - Unloaded existing GameCore instance");
	}

	// load GameCore
	this->game = this->librarian.Load(GAME_CORE);

	// if load failed, we must acqu...I mean abort
	if (!this->game)
	{
		this->log(Logging::LogLevel::LOG_ERROR, "FW - Failed to load GameCore instance");

		return;
	}

	// starting initialization
	this->game->AddArbiter(*this);

	// if we've got clients, let's send em over
	if (!this->clients.empty())
	{
		this->log(Logging::LogLevel::LOG_DEBUG, "FW - Found orphaned clients during hotboot, restoring to GameCore");

		std::vector<fwclient> ccopy;

		for (auto client : this->clients)
		{
			ccopy.push_back(fwclient(client.second));
		}

		this->game->RestoreState(ccopy);
	}

	// finally, finish starting up and notify everyone
	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Setting up the wee babby GameCore");

	this->game->Setup();

	this->log(Logging::LogLevel::LOG_DEBUG, "FW - Hotboot completed successfully");
	this->broadcastMessage(HOTBOOT_STOP_MSG);

	this->gameState = FW::GAME_STATES::FWGAME_RUNNING;

	return;
}
