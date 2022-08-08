namespace FW.Core
{
	public abstract class MigrationBase
	{
		public string Name { get; set; }
		public string Sql { get; set; }


		public MigrationBase()
		{
			return;
		}

		public MigrationBase(string name, string sql)
		{
			this.Name = name;
			this.Sql  = sql;

			return;
		}
	}
}
