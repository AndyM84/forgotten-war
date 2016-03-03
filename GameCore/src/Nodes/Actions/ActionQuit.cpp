#include <Nodes/Actions/ActionQuit.h>

namespace Commands
{
	ActionQuit::ActionQuit()
		: CommandNode("FW::ActionQuit", "0.0.1")
	{ }

	FW::GAME_STATES ActionQuit::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		std::stringstream ss;
		ss << Player->GetName() << " has left the game!";
		Sender.BroadcastToAllButPlayer(Player->GetClient(), ss.str());
		Player->SetState(PLAYER_STATES::PLAYER_DISCONNECTED);

		Sender.SendToClient(Player->GetClient(), "Thanks for playing, come back soon!\n\n");
		Sender.CloseClient(Player->GetClient());

		return World.gameState;
	}
}
