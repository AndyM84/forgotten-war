using System;
using System.Diagnostics;
using System.ServiceProcess;
using System.Threading.Tasks;

namespace FWServ
{
	public partial class FWService : ServiceBase
	{
		public FWService()
		{
			InitializeComponent();
		}

		protected override void OnStart(string[] args)
		{
			Console.WriteLine("HI!");
		}

		protected override void OnStop()
		{
		}

		public void InternalStart()
		{
			OnStart(null);
		}

		public void InternalStop()
		{
			OnStop();
		}
	}
}
