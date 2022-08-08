namespace FW.Core.Migrations
{
	public class _002_CreateAdmins : MigrationBase
	{
		public _002_CreateAdmins()
			: base("002-CreateAdmins",
					@"
INSERT INTO `PlayerChar`
	(`Class`, `Drunk`, `Fatigue`, `Luck`, `Mental`, `Pose`, `Race`, `Created`, `PlayedTime`, `Prompt`,
	`Aliveness`, `Birthdate`, `Citizenship`, `Mortality`, `Name`, `NameLowered`, `PosX`, `PosY`, `PosZ`, `PosVnum`,
	`Password`, `PasswordNeedsChanged`)
	VALUES
	(7, 0.0, 0.0, 0.0, 0.0, 0, 12, NOW(), 0, '`n< %loc% / %stime% >`n',
	0, NOW(), 2, 2, 'Xitan', 'xitan', 0.0, 0.0, 0.0, 1, '', 1),
	(1, 0.0, 0.0, 0.0, 0.0, 0, 12, NOW(), 0, '`n< %loc% / %stime% >`n',
	0, NOW(), 4, 2, 'Kyssandra', 'kyssandra', 0.0, 0.0, 0.0, 1, '', 1);")
		{
			return;
		}
	}
}
