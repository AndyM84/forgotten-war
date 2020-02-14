using System.IO;
using System.Text;

namespace Stoic.Log
{
	public enum FileAppenderOutputTypes
	{
		PLAIN,
		JSON
	}

	public class FileAppender : AppenderBase
	{
		protected string _OutputFile;
		protected FileAppenderOutputTypes _OutputType;


		public FileAppender(string OutputFile, FileAppenderOutputTypes Type)
			: base("FileAppender", "1.0.0")
		{
			this._OutputFile = OutputFile;
			this._OutputType = Type;

			return;
		}

		public override void Process(ref object Sender, ref MessageDispatch Dispatch)
		{
			if (Dispatch.Messages.Count > 0) {
				StringBuilder output = new StringBuilder();

				foreach (var m in Dispatch.Messages) {
					switch (this._OutputType) {
						case FileAppenderOutputTypes.JSON:
							output.AppendLine(m.ToJson());

							break;

						case FileAppenderOutputTypes.PLAIN:
							output.AppendLine(m.ToString());

							break;
					}
				}

				using StreamWriter writer = new StreamWriter(this._OutputFile, true);
				writer.Write(output.ToString());
				writer.Flush();
			}

			return;
		}
	}
}
