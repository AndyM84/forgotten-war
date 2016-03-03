#include <Nodes/Actions/ActionLook.h>

namespace Commands
{
	ActionLook::ActionLook()
		: CommandNode("FW::ActionLook", "0.0.1")
	{ }

	FW::GAME_STATES ActionLook::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		fwbool hasUsers = false;

		std::stringstream ss;
		ss << "A Room\n-----------------\nYou are in a room, just like every other.";

		for (auto plr : World.players)
		{
			if (plr.second->IsInLocation(Player->GetLocation()))
			{
				if (!hasUsers)
				{
					ss << "\n";
					hasUsers = true;
				}

				ss << "\n" << plr.second->GetName() << " is here.";
			}
		}

		Sender.SendToClient(Player->GetClient(), ss.str());

		return World.gameState;
	}
}
