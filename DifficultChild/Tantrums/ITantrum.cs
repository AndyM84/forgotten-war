using Stoic.Log;

namespace DifficultChild.Tantrums
{
	public interface ITantrum
	{
		public void ThrowTantrum(Settings settings, Logger log);
	}
}
