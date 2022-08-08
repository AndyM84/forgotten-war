﻿namespace FW.Core.Migrations
{
	public class _001_CreatePlayerCharTable : MigrationBase
	{
		public _001_CreatePlayerCharTable()
			: base("001-CreatePlayerCharTable",
					@"CREATE TABLE IF NOT EXISTS `PlayerChar` (
`Class` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Drunk` DECIMAL NOT NULL DEFAULT 0.0,
`Fatigue` DECIMAL NOT NULL DEFAULT 0.0,
`Luck` DECIMAL NOT NULL DEFAULT 0.0,
`Mental` DECIMAL NOT NULL DEFAULT 0.0,
`Pose` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Race` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Created` DATETIME NOT NULL,
`PlayedTime` BIGINT UNSIGNED NOT NULL DEFAULT 0,
`Prompt` VARCHAR(512) NOT NULL DEFAULT '',
`Aliveness` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Birthdate` DATETIME NULL,
`Citizenship` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Mortality` TINYINT UNSIGNED NOT NULL DEFAULT 0,
`Name` VARCHAR(24) NOT NULL,
`NameLowered` VARCHAR(24) NOT NULL,
`Vnum` INT UNSIGNED AUTO_INCREMENT,
`PosX` DECIMAL NOT NULL DEFAULT 0.0,
`PosY` DECIMAL NOT NULL DEFAULT 0.0,
`PosZ` DECIMAL NOT NULL DEFAULT 0.0,
`PosVnum` INT UNSIGNED NOT NULL DEFAULT 0,
`Password` VARCHAR(512) NOT NULL,
`PasswordNeedsChanged` TINYINT UNSIGNED NOT NULL DEFAULT 0,
PRIMARY KEY (`Vnum`));")
		{
			return;
		}
	}
}
