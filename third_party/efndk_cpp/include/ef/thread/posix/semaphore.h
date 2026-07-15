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


#ifndef _Semaphore_Posix_
#define _Semaphore_Posix_

//#include <semaphore.h> //g++: sem_t not defined
#include <semaphore.h>
#include <time.h>

#include "../threadbase.h"

namespace EF_Thread
{

	class Semaphore : public ISemaphore
	{
	private:
		sem_t sem;

	public:
		Semaphore( int initValue = 0 )
		{
			sem_init(&sem, 0, initValue); 
		}

		virtual ~Semaphore()
		{
			sem_destroy(&sem);
		}

		void wait()
		{
			sem_wait((sem_t*) &sem);
		}

		// 袁晓辉 添加 2007-8-2
		// 未编译即测试
		bool wait(unsigned long timeout)
		{
			timespec spec = {0};
			if(clock_gettime(CLOCK_REALTIME, &spec) !=  0)
				return false;
			spec.tv_nsec += (timeout * 1000 * 1000);
			
			sem_timedwait(&sem, &spec);
		}

		int tryWait()
		{
			return (sem_trywait((sem_t*) &sem) ? errno : 0);
		}

		int post()
		{
			return (sem_post((sem_t*) &sem) ? errno : 0);
		}

		int value()
		{
			int v = 0;
			sem_getvalue((sem_t*) &sem, &v); 
			return v;
		}

		void reset( int initValue = 0 )
		{
			sem_destroy(&sem);
			sem_init(&sem, 0, initValue);
		}
	};

} //namespace EF_Thread

#endif // !_Semaphore_Posix_
