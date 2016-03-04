#include <Nodes/Emotes/EmoteEmote.h>

namespace Commands
{
	EmoteEmote::EmoteEmote()
		: CommandNode("FW::EmoteEmote", "0.0.1")
	{ }

	FW::GAME_STATES EmoteEmote::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		auto cmd = Message->GetCmd();

		std::stringstream ss, ss2;

		if (cmd == "smile")
		{
			ss << Player->GetName() << " smiles broadly.";
			ss2 << "You smile broadly.";
		}
		else if (cmd == "grin")
		{
			ss << Player->GetName() << "'s mouth spreads into a cheesy grin.";
			ss2 << "You grin widely.";
		}
		else if (cmd == "bow")
		{
			ss << Player->GetName() << " bows slightly.";
			ss2 << "You bow slightly.";
		}
		else if (cmd == "laugh")
		{
			ss << Player->GetName() << " laughs heartily.";
			ss2 << "You laugh heartily.";
		}
		else if (cmd == "chuckle")
		{
			ss << Player->GetName() << " chuckles softly.";
			ss2 << "You chuckle softly.";
		}
		else
		{
			ss << Player->GetName() << " " << Message->GetSansCmd();
			ss2 << "You " << Message->GetSansCmd();
		}

		Sender.SendToClient(Player->GetClient(), ss2.str());

		for (auto plr : World.players)
		{
			if (plr.first != Player->GetID() && plr.second->IsInLocation(Player->GetLocation()))
			{
				Sender.SendToClient(plr.second->GetClient(), ss.str());
			}
		}

		return World.gameState;
	}
}
