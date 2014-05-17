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

	public class DescriptorList : IDictionary<int, Descriptor>
	{
		public void Add(int key, Descriptor value)
		{
			throw new NotImplementedException();
		}

		public bool ContainsKey(int key)
		{
			throw new NotImplementedException();
		}

		public ICollection<int> Keys
		{
			get { throw new NotImplementedException(); }
		}

		public bool Remove(int key)
		{
			throw new NotImplementedException();
		}

		public bool TryGetValue(int key, out Descriptor value)
		{
			throw new NotImplementedException();
		}

		public ICollection<Descriptor> Values
		{
			get { throw new NotImplementedException(); }
		}

		public Descriptor this[int key]
		{
			get
			{
				throw new NotImplementedException();
			}
			set
			{
				throw new NotImplementedException();
			}
		}

		public void Add(KeyValuePair<int, Descriptor> item)
		{
			throw new NotImplementedException();
		}

		public void Clear()
		{
			throw new NotImplementedException();
		}

		public bool Contains(KeyValuePair<int, Descriptor> item)
		{
			throw new NotImplementedException();
		}

		public void CopyTo(KeyValuePair<int, Descriptor>[] array, int arrayIndex)
		{
			throw new NotImplementedException();
		}

		public int Count
		{
			get { throw new NotImplementedException(); }
		}

		public bool IsReadOnly
		{
			get { throw new NotImplementedException(); }
		}

		public bool Remove(KeyValuePair<int, Descriptor> item)
		{
			throw new NotImplementedException();
		}

		public IEnumerator<KeyValuePair<int, Descriptor>> GetEnumerator()
		{
			throw new NotImplementedException();
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			throw new NotImplementedException();
		}
	}
}
