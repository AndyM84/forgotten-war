#include <Logging/LogWorker.h>

namespace Logging
{
	LogWorker::AppenderList *LogWorker::m_Appenders = NULL;
	LogWorker::MessageList *LogWorker::m_Messages = NULL;
	Threading::LockMutex LogWorker::m_Lock = Threading::LockMutex();
	LogWorker *LogWorker::m_WorkerInstance = NULL;

	LogWorker &LogWorker::GetWorker()
	{
		m_Lock.Block();

		if (!m_WorkerInstance)
		{
			m_WorkerInstance = new LogWorker();
		}

		m_Lock.Release();

		return *m_WorkerInstance;
	}

	fwvoid LogWorker::KillWorker()
	{
		m_WorkerInstance->m_Running = false;
		m_Lock.Block();

		if (m_Appenders && m_Messages && m_Appenders->size() > 0 && m_Messages->size() > 0)
		{
			for (MessageListIter mi = m_Messages->begin(); mi != m_Messages->end();)
			{
				for (AppenderListIter ai = m_Appenders->begin(); ai != m_Appenders->end(); ++ai)
				{
					(*ai)->DoAppend(LogData(mi->GetKey(), mi->GetMsg(), mi->GetLevel()));
				}

				mi = m_Messages->erase(mi);
			}

			m_Messages->clear();

			for (AppenderListIter ai = m_Appenders->begin(); ai != m_Appenders->end();)
			{
				ai = m_Appenders->erase(ai);
			}

			m_Appenders->clear();
		}

		if (m_WorkerInstance)
		{
			delete m_WorkerInstance;
			m_WorkerInstance = NULL;
		}

		m_Lock.Release();

		return;
	}

	LogWorker &LogWorker::AddAppender(AppenderBase *appender)
	{
		m_Lock.Block();

		if (!m_Appenders)
		{
			m_Appenders = new AppenderList();
		}

		m_Appenders->push_back(appender);
		m_Lock.Release();

		return *m_WorkerInstance;
	}

	LogWorker &LogWorker::ChunkSize(fwint size)
	{
		if (!m_WorkerInstance)
		{
			return *m_WorkerInstance;
		}

		m_Lock.Block();
		m_WorkerInstance->m_ChunkSize = size;
		m_Lock.Release();

		return *m_WorkerInstance;
	}

	LogWorker &LogWorker::IntervalTime(fwuint milliseconds)
	{
		m_Lock.Block();
		m_WorkerInstance->m_IntervalTime = milliseconds;
		m_Lock.Release();

		return *m_WorkerInstance;
	}

	LogWorker &LogWorker::AddMessage(LogData *message)
	{
		m_Lock.Block();

		if (!m_Messages)
		{
			m_Messages = new MessageList();
		}

		m_Messages->push_back(LogData(message->GetKey(), message->GetMsg(), message->GetLevel()));
		m_Lock.Release();

		return *m_WorkerInstance;
	}

	fwvoid LogWorker::run()
	{
		if (!m_Appenders || !m_WorkerInstance)
		{
			return;
		}

		m_WorkerInstance->m_Running = true;

		while (m_WorkerInstance->m_Running)
		{
			MessageList *chunk = new MessageList();
			int count = 0;

			m_Lock.Block();

			if (m_Messages)
			{
				for (MessageListIter mi = m_Messages->begin(); mi != m_Messages->end();)
				{
					if (count == m_WorkerInstance->m_ChunkSize)
					{
						break;
					}

					chunk->push_back(LogData(mi->GetKey(), mi->GetMsg(), mi->GetLevel()));
					mi = m_Messages->erase(mi);

					++count;
				}
			}

			m_Lock.Release();

			if (count > 0)
			{
				for (MessageListIter mi = chunk->begin(); mi != chunk->end();)
				{
					for (AppenderListIter ai = m_Appenders->begin(); ai != m_Appenders->end(); ++ai)
					{
						(*ai)->DoAppend(LogData(mi->GetKey(), mi->GetMsg(), mi->GetLevel()));
					}

					mi = chunk->erase(mi);
				}
			}

			this->Millisleep(m_WorkerInstance->m_IntervalTime);
		}

		return;
	}

	LogWorker::LogWorker()
	{
		this->m_Running = true;
		this->m_ChunkSize = 10;
		this->m_IntervalTime = 1000;

		return;
	}
};
