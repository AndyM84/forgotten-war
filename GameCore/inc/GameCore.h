#pragma once

#include <CommonCore.h>

class GameCore : public Libraries::GameLibrary
{
public:
	virtual fwbool Setup();
	virtual fwbool Destroy();
	virtual fwvoid Run();
	virtual fwuint ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwvoid ClientReceived(fwuint ID, const fwstr Message);
	virtual fwuint ClientDisconnected(fwuint ID, const sockaddr_in Address);
};
