#include <Nodes/Info/InfoWho.h>

namespace Commands
{
	InfoWho::InfoWho()
		: CommandNode("FW::InfoWho", "0.0.1")
	{ }

	FW::GAME_STATES InfoWho::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		std::stringstream ss;
		ss << "\n`w[`RWho's Online`w]\n`w------------\n";

		for (auto pl : World.players) {
			if (pl.second->GetState() != PLAYER_STATES::PLAYER_CONNECTED) {
				continue;
			}

			ss << "`W" << pl.second->GetName();

			if (pl.first == Player->GetID()) {
				ss << " `K(`cYou`K)";
			}

			ss << "\n";
		}

		ss << "\n";
		Sender.SendToClient(Player->GetClient(), ss.str());

		return World.gameState;
	}
}
