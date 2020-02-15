namespace Stoic.Log
{
	public class NullAppender : AppenderBase
	{
		public NullAppender()
			: base("NullAppender", "1.0.0")
		{
			return;
		}


		public override void Process(ref object Sender, ref MessageDispatch Dispatch)
		{
			return;
		}
	}
}
