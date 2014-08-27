#define _CRT_SECURE_NO_WARNINGS

#include <fw.h>

// TODO: Implement Socket stuff...
//       Windows: http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5e.html
//       Linux: http://www.tidytutorials.com/2012/06/linux-c-overlapped-server-and-client.html

int main()
{
	Server::FWServer *serv = new Server::FWServer("127.0.0.1", 4000);
	serv->Start();

	while (serv->IsListening())
	{
		serv->Update();
	}

	return 0;
}
