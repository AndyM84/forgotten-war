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
