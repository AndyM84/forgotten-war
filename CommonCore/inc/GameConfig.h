#pragma once

#include <Common/Types.h>

#include <string>
#include <iostream>

struct GameDbSettings
{
	fwstr connectionString;
	fwstr tablePrefix;
};

struct GameAdminSettings
{
	fwstr hotfixPassword;
	fwstr shutdownPassword;
};

class GameConfig
{
public:
	GameConfig(fwstr exePath, GameDbSettings dbSettings, GameAdminSettings adminSettings);

	const fwstr GetExePath() const;
	const GameDbSettings GetDbSettings() const;
	const GameAdminSettings GetAdminSettings() const;
	const fwstr ToJson() const;
	const fwstr ToJson(fwbool formatted) const;

protected:
	const fwstr m_exePath;
	const GameDbSettings m_dbSettings;
	const GameAdminSettings m_adminSettings;
};
