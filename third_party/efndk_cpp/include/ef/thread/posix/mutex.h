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


#ifndef _Mutex_Posix_
#define _Mutex_Posix_

#include <pthread.h>

namespace EF_Thread
{

	class Mutex : public IMutex
	{
	private:
		pthread_mutex_t m_mutex;
		void operator=(Mutex &m) {}
		Mutex(const Mutex &m) {}

	public:
		Mutex()
		{
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m_mutex, &attr);
			pthread_mutexattr_destroy(&attr);
		}

		virtual ~Mutex()
		{
			pthread_mutex_unlock(&m_mutex);
			pthread_mutex_destroy(&m_mutex);
		}

		int lock()
		{
			return pthread_mutex_lock(&m_mutex);
		}

		/*
		int tryLock()
		{
			return pthread_mutex_trylock(&m_mutex);
		}
		*/

		int unlock()
		{
			return pthread_mutex_unlock(&m_mutex);
		}
	};

} //namespace EF_Thread

#endif // !_Mutex_Posix_
