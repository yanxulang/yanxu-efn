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


//threadbase.h
#ifndef _THREADBASE_H_
#define _THREADBASE_H_

namespace EF_Thread
{
	class IMutex
	{
	private:
		//防止赋值和拷贝构造操作
		void operator=(IMutex &M) {}
		IMutex(IMutex &M) {}

	public:
		IMutex(){};
		virtual ~IMutex(){};
		virtual int lock() = 0;
		//virtual int tryLock() = 0;
		virtual int unlock() = 0;
	};


	class ISemaphore
	{
	private:
		void operator= (ISemaphore &semaphore) {}
		ISemaphore(ISemaphore &semaphore) {}

	public:
		ISemaphore(){}
		virtual ~ISemaphore(){}
		virtual void wait() = 0;
		// 返回是否没有超时
		virtual bool wait(unsigned long timeout) = 0;
		virtual int tryWait() = 0;
		virtual int post() = 0;
		virtual int value() = 0;
		virtual void reset(int initValue = 0) = 0;
	};

#ifdef WIN32
	#define THREAD_FUNCTION WINAPI
#else
	#define THREAD_FUNCTION
#endif

	class IThread
	{
	public:
		virtual ~IThread() {}
		virtual int create(int nStackSize = 0) = 0;
		virtual int wait(void) = 0;
		virtual int terminate() = 0;

	protected:
		virtual void main(void) = 0;

		virtual IMutex& getMutex() = 0;
		virtual ISemaphore& getSemaphore() = 0;

	protected:
		static void* THREAD_FUNCTION ThreadMainHandler(IThread* pThread)
		{
			if(pThread)
			{
				pThread->main();
			}
			return(0);
		}
	};

	void sleep(unsigned int millisec);

}//namespace EF_Thread

#endif //_THREADBASE_H_
