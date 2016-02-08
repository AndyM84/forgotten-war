#pragma once

#include <CommonCore.h>
#include <iostream>

class GameCore : public Libraries::GameLibrary
{
public:
	virtual fwbool Setup();
	virtual fwbool Destroy();
	virtual fwvoid Run();
	virtual fwvoid SaveState();
	virtual fwclient *RestoreState();
	virtual fwbool ClientIsAdmin(fwuint ID);
	virtual fwvoid AddCallbacks(const FWSendMethod &send);
	virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwclient ClientReceived(fwuint ID, const fwstr Message);
	virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address);
};
