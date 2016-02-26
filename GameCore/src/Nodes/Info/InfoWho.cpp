#include <Nodes/Info/InfoWho.h>

namespace Commands
{
	InfoWho::InfoWho()
		: CommandNode("FW::InfoWho", "0.0.1")
	{ }

	FW::GAME_STATES InfoWho::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		std::stringstream ss;
		ss << "\nWho's Online\n------------\n";

		for (auto pl : World.players)
		{
			ss << pl.second->GetName();

			if (pl.first == Player->GetID())
			{
				ss << " (You)";
			}

			ss << "\n";
		}

		ss << "\n\n";
		Sender.SendToClient(Player->GetClient(), ss.str());

		return World.gameState;
	}
}
