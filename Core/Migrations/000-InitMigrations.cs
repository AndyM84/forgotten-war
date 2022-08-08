namespace FW.Core.Migrations
{
	public class _000_InitMigrations : MigrationBase
	{
		public _000_InitMigrations()
			: base("000-InitMigrations",
					"CREATE TABLE IF NOT EXISTS `Migration` (`ID` INT UNSIGNED NOT NULL AUTO_INCREMENT, `FileName` VARCHAR(256) NOT NULL, PRIMARY KEY (`ID`))")
		{
			return;
		}
	}
}
