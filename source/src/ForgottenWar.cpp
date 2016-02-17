#include <ForgottenWar.h>

/* Ctor's and dtor */

ForgottenWar::ForgottenWar(fwuint Port)
{
	this->logger = &Logging::Logger::GetLogger("FW");
	this->server = new Server::SelectServer(*this, Port, *this->logger);
	this->librarian = new Libraries::Librarian<Libraries::GameLibrary>();

	return;
}

ForgottenWar::ForgottenWar(fwuint Port, Logging::Logger &Logger)
{
	this->logger = &Logger;
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

	return;
}

/* Server::ServerListener methods */

fwvoid ForgottenWar::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	return;
}

fwvoid ForgottenWar::ClientReceived(fwuint ID, const Server::SocketMessage &Message)
{
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
