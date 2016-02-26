#include <Nodes/Comms/CommOoc.h>

namespace Commands
{
	CommOoc::CommOoc()
		: CommandNode("FW::CommOoc", "0.0.1")
	{ }

	FW::GAME_STATES CommOoc::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		if (Message->GetTokens().size() < 2)
		{
			return World.gameState;
		}

		std::stringstream ss;
		ss << "[OOC] " << Player->GetName() << ": " << Message->GetSansCmd();

		Sender.BroadcastToAll(ss.str());
		Message->Consume();

		return World.gameState;
	}
}
