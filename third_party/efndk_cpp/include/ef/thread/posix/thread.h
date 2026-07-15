/*
    版权所有 (C) 2007 大连大有吴涛易语言软件开发有限公司

    在满足下列所有许可条件的前提下，允许以源代码形式或编译后形式使用或重新发布本源文件的未经修改版本或修改后版本：
    1、如果以源代码形式重新发布，必须完整保留作者版权声明。
    2、如果以编译后形式重新发布，必须在发布版本的文档中重申作者版权声明。
    3、如果重新发布修改后版本，必须注明修改内容，必须完整保留或重申原作者版权声明。
    4、对本源文件的任何使用，必须满足以下要求之一：
       A、开发“易语言.飞扬”类库；
       B、开发用于“易语言.飞扬”的工具软件；
       C、开发用作辅助已有“易语言.飞扬”程序的软件。
    5：本源文件的作者不作任何承诺。任何情况下因任何原因而导致的任何损害，均不承担任何责任。
    6：必须保证本协议的完整性。
*/


#ifndef _Thread_Posix_
#define _Thread_Posix_

#include "../threadbase.h"
#include "semaphore.h"
#include "mutex.h"
#include <stdio.h>

#include <pthread.h>


namespace EF_Thread
{
	class MutexLockerForCreateThread
	{
	private:
		IMutex& m_mutex;
	public:
		MutexLockerForCreateThread(IMutex& mutex) : m_mutex(mutex)
		{
			m_mutex.lock();
		}

		~MutexLockerForCreateThread()
		{
			m_mutex.unlock();
		}
	};

	
	typedef pthread_t Handle;

	class Thread : public IThread		
	{
	private:
		IMutex& getMutex() { static Mutex m; return (IMutex&)m; }
		ISemaphore& getSemaphore()  { static Semaphore s; return (ISemaphore&)s; }

	protected:
		Handle m_handle;

	public:
		Thread() : m_handle(0)
		{}

		virtual ~Thread()
		{
			if(m_handle) pthread_detach(m_handle); //?
		}

		Handle getHandle(void)
		{
			return m_handle;
		}

		int create(int nStackSize = 0)
		{
			MutexLockerForCreateThread locker(getMutex());

			pthread_attr_t attr;
			pthread_attr_init(&attr);

			if (nStackSize)
				pthread_attr_setstacksize(&attr, nStackSize);

			int result = pthread_create(&m_handle, &attr, (void*(*)(void*))ThreadMainHandler, (void*)this);

			pthread_attr_destroy(&attr);

			return result;
		}

		int wait(void)
		{
			if(m_handle)
				return pthread_join(m_handle, 0); 
			else
				return 0;
		}

		int terminate()
		{
			if(m_handle)
				return pthread_cancel(m_handle); 
			else
				return 0;
		}

	};

	inline void sleep(unsigned int millisec)
	{
		timespec tmReq;

		tmReq.tv_sec  = (time_t)(millisec / 1000);
		tmReq.tv_nsec = (time_t)(millisec % 1000) * 1000000;
		
		nanosleep(&tmReq, (timespec*)NULL);
	}

	inline Handle getCurrentThreadId()
	{
		return (Handle) pthread_self();
	}

}//namespace EF_Thread
#endif // !_Thread_Posix_
