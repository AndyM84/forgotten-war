#include <ForgottenWar.h>

/* Ctor's and dtor */

ForgottenWar::ForgottenWar(fwuint Port)
{
	this->logger = std::make_shared<Logging::Logger>(&Logging::Logger::GetLogger("FW"));
	this->server = new Server::SelectServer(*this, Port, *this->logger);
	this->librarian = new Libraries::Librarian<Libraries::GameLibrary>();

	return;
}

ForgottenWar::ForgottenWar(fwuint Port, Logging::Logger &Logger)
{
	this->logger = std::make_shared<Logging::Logger>(Logger);
	this->server = new Server::SelectServer(*this, Port, *this->logger);
	this->librarian = new Libraries::Librarian<Libraries::GameLibrary>();

	return;
}

ForgottenWar::~ForgottenWar()
{
	if (this->game)
	{
		delete this->game;
	}

	if (this->librarian)
	{
		delete this->librarian;
	}

	this->logger.reset();

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

	auto clientIter = this->clients.find(ID);

	auto msg = ServerMessage();
	msg.Initialize(Message.Message);

	if (clientIter != this->clients.end() && msg.IsValid())
	{

	}

	return;
}

fwvoid ForgottenWar::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	return;
}

/* ForgottenWar::CoreArbiter methods */

fwvoid ForgottenWar::SendToClient(fwuint ID, fwstr Message)
{
	return;
}

fwvoid ForgottenWar::CloseClient(fwuint ID)
{
	return;
}

fwvoid ForgottenWar::SendLog(Logging::LogLevel Level, const fwchar *Message)
{
	return;
}

fwvoid ForgottenWar::Initialize()
{
	return;
}

fwvoid ForgottenWar::GameLoop()
{
	return;
}

/* Protected methods */

fwvoid ForgottenWar::log(Logging::LogLevel Level, const fwchar *Message)
{
	return;
}

fwvoid ForgottenWar::broadcastMessage(fwstr Message)
{
	return;
}

fwvoid ForgottenWar::broadcastMessageToOthers(fwuint ID, fwstr Message)
{
	return;
}
