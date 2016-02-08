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

fwvoid GameCore::SaveState()
{


	return;
}

fwclient *GameCore::RestoreState()
{
	return NULL;
}

fwbool GameCore::ClientIsAdmin(fwuint ID)
{
	return true;
}

fwvoid GameCore::AddCallbacks(const FWSendMethod &send)
{
	this->send = &send;

	return;
}

fwclient GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	return fwclient { ID, 0, Address, CCLIENT_CONNECTED };
}

fwclient GameCore::ClientReceived(fwuint ID, const fwstr Message)
{
	//std::stringstream ss;
	//ss << "You send the following message: " << Message << "\n\n";
	//this->send(ID, ss.str());

	return fwclient { ID, 0, NULL, CCLIENT_CONNECTED };
}

fwclient GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	return fwclient { ID, 0, Address, CCLIENT_INVALID };
}

FW_INIT_LIBRARY(GameCore);
