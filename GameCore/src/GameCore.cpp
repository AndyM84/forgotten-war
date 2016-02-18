#include <GameCore.h>

/* Destructor */

GameCore::~GameCore()
{
	return;
}

/* Libraries::Library methods */

fwbool GameCore::Setup()
{
	return true;
}

fwbool GameCore::Destroy()
{
	return true;
}

/* Libraries::GameLibrary methods */

FW::GAME_STATES GameCore::GameLoop(fwfloat Delta)
{
	
}

fwvoid GameCore::SaveState()
{
	return;
}

fwvoid GameCore::RestoreState(std::vector<fwclient> clients)
{
	return;
}

fwvoid GameCore::AddArbiter(FW::CoreArbiter &send)
{
	return;
}

fwclient GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{

}

fwclient GameCore::ClientReceived(fwuint ID, ServerMessage Message)
{

}

fwclient GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{

}

/* GameCore methods */

fwvoid GameCore::Log(const Logging::LogLevel Level, const fwchar *Message)
{
	return;
}

fwvoid GameCore::SendToClient(const fwclient Client, const fwstr Message)
{
	return;
}

fwvoid GameCore::CloseClient(const fwclient Client)
{
	return;
}

fwvoid GameCore::BroadcastToAllButPlayer(const std::shared_ptr<Player> Client, const fwstr Message)
{
	return;
}

fwvoid GameCore::BroadcastToAll(const fwstr Message)
{
	return;
}

std::vector<fwclient> GameCore::GetClients()
{
	
}

FW_INIT_LIBRARY(GameCore);
