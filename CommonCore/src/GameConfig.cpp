#include <GameConfig.h>

GameConfig::GameConfig(fwstr exePath, GameDbSettings dbSettings, GameAdminSettings adminSettings)
	: m_exePath(exePath), m_dbSettings(dbSettings), m_adminSettings(adminSettings)
{ }

const fwstr GameConfig::GetExePath() const
{
	return this->m_exePath;
}

const GameDbSettings GameConfig::GetDbSettings() const
{
	return this->m_dbSettings;
}

const GameAdminSettings GameConfig::GetAdminSettings() const
{
	return this->m_adminSettings;
}

const fwstr GameConfig::ToJson() const
{
	return this->ToJson(false);
}

const fwstr GameConfig::ToJson(fwbool formatted) const
{
	if (formatted)
	{
		fwstr json("\t{\n\t\t\"exePath\": \"");
		json += this->m_exePath;
		json += "\",\n\t\t\"dbSettings\": {\n\t\t\t\"connectionString\": \"";
		json += this->m_dbSettings.connectionString;
		json += "\",\n\t\t\t\"tablePrefix\": \"";
		json += this->m_dbSettings.tablePrefix;
		json += "\"\n\t\t},\n\t\t\"adminSettings\": {\n\t\t\t\"hotfixPassword\": \"";
		json += this->m_adminSettings.hotfixPassword;
		json += "\",\n\t\t\t\"shutdownPassword\": \"";
		json += this->m_adminSettings.shutdownPassword;
		json += "\"\n\t\t}\n\t}";

		return json;
	}

	fwstr json("{ \"exePath\": \"");
	json += this->m_exePath;
	json += "\", \"dbSettings\": { \"connectionString\": \"";
	json += this->m_dbSettings.connectionString;
	json += "\", \"tablePrefix\": \"";
	json += this->m_dbSettings.tablePrefix;
	json += "\" }, \"adminSettings\": { \"hotfixPassword\": \"";
	json += this->m_adminSettings.hotfixPassword;
	json += "\", \"shutdownPassword\": \"";
	json += this->m_adminSettings.shutdownPassword;
	json += "\" } }";

	return json;
}
