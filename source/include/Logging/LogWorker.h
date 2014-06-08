#pragma once

#include <Common/Types.h>
#include <Threading/Threadable.h>
#include <Threading/LockMutex.h>

#include "LogData.h"
#include "AppenderBase.h"

#include <iostream>
#include <vector>

namespace Logging
{
	class LogWorker : public Threading::Threadable
	{
	public:
		static LogWorker &GetWorker();
		static fwvoid KillWorker();

		static LogWorker &AddAppender(AppenderBase *appender);
		static LogWorker &ChunkSize(fwint size);
		static LogWorker &IntervalTime(fwuint milliseconds);
		static LogWorker &AddMessage(LogData *message);

		virtual fwvoid run();

	protected:
		LogWorker();

		typedef std::vector<AppenderBase*> AppenderList;
		typedef std::vector<AppenderBase*>::iterator AppenderListIter;
		typedef std::vector<LogData> MessageList;
		typedef std::vector<LogData>::iterator MessageListIter;

		static AppenderList *m_Appenders;
		static MessageList *m_Messages;
		static Threading::LockMutex m_Lock;
		static LogWorker *m_WorkerInstance;

		bool m_Running;
		int m_ChunkSize;
		fwuint m_IntervalTime;
	};
};
