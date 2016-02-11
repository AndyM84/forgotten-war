#pragma once

#include <CommonCore.h>
#include <iostream>

// Utility macros
#define UMIN(a, b)               ((a) < (b) ? (a) : (b))
#define UMAX(a, b)               ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)          ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define IS_SET(flag, bit)        ((flag) & (bit))
#define SET_BIT(var, bit)        ((var) |= (bit))
#define REMOVE_BIT(var, bit)     ((var) &= ~(bit))

class GameCore : public Libraries::GameLibrary
{
public:
	virtual fwbool Setup();
	virtual fwbool Destroy();
	virtual fwvoid Run();
	virtual fwvoid SaveState();
	virtual fwvoid RestoreState(std::vector<fwclient> clients);
	virtual fwbool ClientIsAdmin(fwuint ID);
	virtual fwvoid AddCallbacks(FWSender &send);
	virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwclient ClientReceived(fwuint ID, const fwstr Message);
	virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address);
};
