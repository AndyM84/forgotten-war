#include <GameCore.h>

fwbool GameCore::Setup()
{
	return false;
}

fwbool GameCore::Destroy()
{
	return false;
}

fwvoid GameCore::Run()
{
	return;
}

fwvoid GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	return;
}

fwvoid GameCore::ClientReceived(fwuint ID, const fwstr Message)
{
	return;
}

fwvoid GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	return;
}

FW_INIT_LIBRARY(GameCore);
