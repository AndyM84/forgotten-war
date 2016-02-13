#pragma once

#include <CommonCore.h>

#include <iostream>
#include <map>

// Utility macros
#define UMIN(a, b)               ((a) < (b) ? (a) : (b))
#define UMAX(a, b)               ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)          ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define IS_SET(flag, bit)        ((flag) & (bit))
#define SET_BIT(var, bit)        ((var) |= (bit))
#define REMOVE_BIT(var, bit)     ((var) &= ~(bit))

class GameCore : public Libraries::GameLibrary, public Threading::Threadable
{
public:
	virtual fwvoid Run();
	virtual fwbool Setup();
	virtual fwbool Destroy();
	virtual fwvoid GameLoop();
	virtual fwvoid SaveState();
	virtual fwvoid RestoreState(std::vector<fwclient> clients);
	virtual fwbool ClientIsAdmin(fwuint ID);
	virtual fwvoid AddCallbacks(FWSender &send);
	virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwclient ClientReceived(fwuint ID, ServerMessage Message);
	virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address);
	virtual Threading::Threadable &GetThreadable();

	fwvoid SendToClient(const fwclient Client, const fwstr Message) const;
	fwvoid BroadcastToAllButClient(const fwclient Client, const fwstr Message) const;
	fwvoid BroadcastToAll(const fwstr Message) const;
	const fwclient GetClient(fwuint ID) const;
	const std::vector<fwclient> GetClients() const;

protected:
	Threading::LockCriticalSection clientLock;
	std::map<fwuint, fwclient> clients;
	fwbool gameRunning;
};
