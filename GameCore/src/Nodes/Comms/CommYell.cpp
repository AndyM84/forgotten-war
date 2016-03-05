#include <Nodes/Comms/CommYell.h>

namespace Commands
{
	CommYell::CommYell()
		: CommandNode("FW::CommYell", "0.0.1")
	{ }

	FW::GAME_STATES CommYell::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		std::stringstream ss;
		ss << "`yYou yell, '" << Message->GetSansCmd() << "'.";
		Sender.SendToClient(Player->GetClient(), ss.str());

		ss = std::stringstream("");
		ss << "`y" << Player->GetName() << " yells, '" << Message->GetSansCmd() << "'.";

		for (auto plr : World.players)
		{
			if (plr.first == Player->GetID() || !plr.second->IsNearLocation(Player->GetLocation()))
			{
				continue;
			}

			Sender.SendToClient(plr.second->GetClient(), ss.str());
		}

		return World.gameState;
	}
}
