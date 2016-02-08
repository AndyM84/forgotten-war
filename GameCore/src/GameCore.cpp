#include <GameCore.h>

fwbool GameCore::Setup()
{
	std::cout << "I have been setup!" << std::endl;

	return true;
}

fwbool GameCore::Destroy()
{
	std::cout << "I have been destroyed!" << std::endl;

	return true;
}

fwvoid GameCore::Run()
{
	std::cout << "I have been run!" << std::endl;

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

fwuint GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	return 0;
}

FW_INIT_LIBRARY(GameCore);
