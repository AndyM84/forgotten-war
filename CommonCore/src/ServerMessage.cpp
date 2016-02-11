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
	this->sansCmd = (this->tokens.size() > 1) ? this->raw.substr(this->tokens[0].length()) : "";
	this->MakeValid();

	return;
}

const fwstr ServerMessage::GetCmd()
{
	return this->cmd;
}

const fwstr ServerMessage::GetRaw()
{
	return this->raw;
}

const fwstr ServerMessage::GetSansCmd()
{
	return this->sansCmd;
}

const std::vector<fwstr> ServerMessage::GetTokens()
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
