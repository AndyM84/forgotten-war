#include <ServerMessage.h>

ServerMessage::ServerMessage()
{
	this->MakeConsumable();

	return;
}

fwvoid ServerMessage::Initialize()
{
	return;
}

fwvoid ServerMessage::Initialize(const fwstr Message)
{
	if (Message.empty())
	{
		return;
	}

	fwstr tmp;

	for (auto ch : Message)
	{
		if (ch == '\n' || ch == '\r')
		{
			this->hasLinefeed = (ch == '\r') ? true : false;

			break;
		}

		if (ch == ' ')
		{
			this->tokens.push_back(tmp);

			this->raw += tmp;
			this->raw += ' ';

			tmp.clear();

			continue;
		}

		tmp += ch;
	}

	if (!tmp.empty())
	{
		this->tokens.push_back(tmp);
	}

	if (this->tokens.empty())
	{
		return;
	}

	this->cmd = this->tokens[0];
	std::transform(this->cmd.begin(), this->cmd.end(), this->cmd.begin(), ::tolower);

	this->sansCmd = (this->tokens.size() > 1) ? this->raw.substr(this->tokens[0].length()) : "";
	this->MakeValid();

	return;
}

const fwstr ServerMessage::GetCmd() const
{
	return this->cmd;
}

const fwstr ServerMessage::GetRaw() const
{
	return this->raw;
}

const fwstr ServerMessage::GetSansCmd() const
{
	return this->sansCmd;
}

const std::vector<fwstr> ServerMessage::GetTokens() const
{
	return std::vector<fwstr>(this->tokens);
}

fwint ServerMessage::NumResults()
{
	return 0;
}

fwvoid ServerMessage::SetResult()
{
	return;
}
