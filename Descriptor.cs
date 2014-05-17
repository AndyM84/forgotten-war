using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

namespace FWServ
{
	public class Descriptor
	{
		private Queue<string> OutputBuffer;
		private Socket Client;

		public Descriptor(Socket client)
		{
			this.OutputBuffer = new Queue<string>();
			this.Client = client;

			return;
		}

		public void AddOutput(string output)
		{
			this.OutputBuffer.Enqueue(output);

			return;
		}

		public string GetNextFromBuffer()
		{
			if (this.OutputBuffer.Count < 1)
			{
				return null;
			}

			return this.OutputBuffer.Dequeue();
		}
	}

	public class DescriptorArgs : EventArgs
	{
		public Descriptor Descriptor;

		public DescriptorArgs(Descriptor descriptor)
		{
			this.Descriptor = descriptor;

			return;
		}
	}
}
