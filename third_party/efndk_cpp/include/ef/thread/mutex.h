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


#ifndef _Mutex_
#define _Mutex_

#include <errno.h>

#ifdef WIN32
  #include "win32/mutex.h"
#else
  #include "posix/mutex.h"
#endif

namespace EF_Thread
{
	class MutexLock
	{
	private:
		Mutex& m_mutex;
	public:
		MutexLock(Mutex& mutex) : m_mutex(mutex)
		{
			m_mutex.lock();
		}

		~MutexLock()
		{
			m_mutex.unlock();
		}
	};
}

#endif // !_Mutex_
