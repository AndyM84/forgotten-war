using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FWServ
{
	public delegate void DescriptorConnectedEventHandler(object sender, DescriptorArgs args);
	public delegate void DescriptorDisconnectedEventHandler(object sender, DescriptorArgs args);
	public delegate void InputReceivedEventHandler(object sender, string input);
	public delegate void DataRecevedEventHandler(object sender, DescriptorArgs args);
	public delegate void DataSentEventHandler(object sender, DescriptorArgs args);
}
