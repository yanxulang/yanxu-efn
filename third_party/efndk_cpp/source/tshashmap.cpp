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


// tshashmap.cpp
// Thread-Safe Hashmap

#include <ef/tshashmap.h>

//注意: 枚举哈希表成员操作目前仍然不是线程安全的! 需通过GetMutex()手工锁定和解锁。

EF_ThreadSafePtr2PtrHashmap::EF_ThreadSafePtr2PtrHashmap (int nPresumedElementCount)
	: EF_Ptr2PtrHashmap(nPresumedElementCount)
{
}

EFBool EF_ThreadSafePtr2PtrHashmap::Get (void* key, void** pValue)
{
	EF_Thread::MutexLock lock(m_Mutex);
	return EF_Ptr2PtrHashmap::Get(key, pValue);
}

void EF_ThreadSafePtr2PtrHashmap::Set (void* key, void* value)
{
	EF_Thread::MutexLock lock(m_Mutex);
	EF_Ptr2PtrHashmap::Set(key, value);
}

void EF_ThreadSafePtr2PtrHashmap::Remove (void* key)
{
	EF_Thread::MutexLock lock(m_Mutex);

	void* keyInIterator;
	if(((iterator*)m_pIterator)->GetKey(&keyInIterator) 
		&& m_pfn_IsKeyEquals(keyInIterator, key))
	{
		m_pIterator->skip();
	}

	EF_Ptr2PtrHashmap::Remove(key);
}

void EF_ThreadSafePtr2PtrHashmap::RemoveAll (void)
{
	EF_Thread::MutexLock lock(m_Mutex);
	EF_Ptr2PtrHashmap::RemoveAll();
}

const EF_Thread::Mutex& EF_ThreadSafePtr2PtrHashmap::GetMutex()
{
	return m_Mutex;
}
