﻿using System;
using System.ServiceProcess;

using FWServ;

namespace ServiceTester
{
	class Program
	{
		static void Main(string[] args)
		{
			var svc = new FWService();
			svc.InternalStart();

			Console.ReadLine();

			return;
		}
	}
}
