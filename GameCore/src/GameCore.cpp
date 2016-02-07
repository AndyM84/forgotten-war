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

fwuint GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	return 0;
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
