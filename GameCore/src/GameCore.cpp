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

fwvoid GameCore::RestoreState(std::vector<fwclient> clients)
{
	return;
}

fwbool GameCore::ClientIsAdmin(fwuint ID)
{
	return true;
}

fwvoid GameCore::AddCallbacks(FWSender &send)
{
	this->sender = &send;

	return;
}

fwclient GameCore::ClientConnected(fwuint ID, const sockaddr_in Address)
{
	fwuint nId = 0;

	for (auto client : this->clients)
	{
		if (client.second.plyrid >= nId)
		{
			nId = client.second.plyrid + 1;
		}
	}

	this->clients.insert(std::pair<const fwuint, fwclient>(nId, fwclient { ID, nId, Address, CCLIENT_CONNECTED }));

	return fwclient { ID, nId, Address, CCLIENT_CONNECTED };
}

fwclient GameCore::ClientReceived(fwuint ID, ServerMessage Message)
{
	if (!Message.IsValid())
	{
		return fwclient{ ID, 0, NULL, CCLIENT_INVALID };
	}

	auto client = this->GetClient(ID);

	std::stringstream ss;
	ss << "You sent the following message: " << Message.GetRaw();
	this->SendToClient(client, ss.str());

	return client;
}

fwclient GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	auto client = this->GetClient(ID);
	auto iter = this->clients.find(ID);
	this->clients.erase(iter);

	return fwclient { ID, client.plyrid, Address, CCLIENT_INVALID };
}

fwvoid GameCore::SendToClient(const fwclient Client, const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	fwstr tmp;

	if (Message[0] != '\n' && Message[0] != '\r')
	{
		tmp += "\n\n";
	}

	tmp += Message;

	if (Message[Message.length() - 1] != ' ' && Message[Message.length() - 1] != '\n' && Message[Message.length() - 2] != '\r')
	{
		tmp += ' ';
	}

	this->sender->sendToClient(Client.sockfd, tmp);

	return;
}

fwvoid GameCore::BroadcastToAllButClient(const fwclient Client, const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	for (auto c : this->clients)
	{
		if (c.second.plyrid != Client.plyrid)
		{
			this->SendToClient(c.second, Message);
		}
	}

	return;
}

fwvoid GameCore::BroadcastToAll(const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	for (auto client : this->clients)
	{
		this->SendToClient(client.second, Message);
	}

	return;
}

const fwclient GameCore::GetClient(fwuint ID) const
{
	auto client = this->clients.find(ID);

	if (client == this->clients.end())
	{
		return fwclient();
	}

	return (*client).second;
}

const std::vector<fwclient> GameCore::GetClients() const
{
	std::vector<fwclient> tmp;

	for (auto client : this->clients)
	{
		tmp.push_back(client.second);
	}

	return tmp;
}

FW_INIT_LIBRARY(GameCore);
