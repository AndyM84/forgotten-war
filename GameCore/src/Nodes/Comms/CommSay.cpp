#include <Nodes/Comms/CommSay.h>

namespace Commands
{
	CommSay::CommSay()
		: CommandNode("FW::CommSay", "0.0.1")
	{ }

	FW::GAME_STATES CommSay::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		std::stringstream ss;
		ss << "`gYou say, '" << Message->GetSansCmd() << "'.";
		Sender.SendToClient(Player->GetClient(), ss.str());

		ss = std::stringstream("");
		ss << "`g" << Player->GetName() << " says, '" << Message->GetSansCmd() << "'.";

		for (auto plr : World.players)
		{
			if (plr.first == Player->GetID() || !plr.second->IsInLocation(Player->GetLocation()))
			{
				continue;
			}

			Sender.SendToClient(plr.second->GetClient(), ss.str());
		}

		return World.gameState;
	}
}
