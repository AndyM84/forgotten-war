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
		if (client.second.plyrid > nId)
		{
			nId = client.second.plyrid + 1;
		}
	}

	this->clients.insert(std::pair<const fwuint, fwclient>(nId, fwclient { ID, nId, Address, CCLIENT_CONNECTED }));

	return fwclient { ID, 0, Address, CCLIENT_CONNECTED };
}

fwclient GameCore::ClientReceived(fwuint ID, ServerMessage Message)
{
	if (!Message.IsValid())
	{
		return fwclient{ ID, 0, NULL, CCLIENT_CONNECTED };
	}

	std::stringstream ss;
	ss << "You send the following message: " << Message.GetRaw() << "\n\n";
	this->SendToClient(this->GetClient(ID), ss.str());

	return fwclient { ID, 0, NULL, CCLIENT_CONNECTED };
}

fwclient GameCore::ClientDisconnected(fwuint ID, const sockaddr_in Address)
{
	return fwclient { ID, 0, Address, CCLIENT_INVALID };
}

fwvoid GameCore::SendToClient(const fwclient Client, const fwstr Message) const
{
	if (Message.empty())
	{
		return;
	}

	fwstr tmp;

	if (tmp[0] != '\n' && tmp[0] != '\r')
	{
		tmp += "\n\n";
	}

	tmp += Message;

	this->sender->sendToClient(Client.sockfd, tmp);

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
