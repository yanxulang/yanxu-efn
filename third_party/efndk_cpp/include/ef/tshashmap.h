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


#ifndef __EF_TS_HASH_MAP_H__
#define __EF_TS_HASH_MAP_H__

// tshashmap.h
// Thread-Safe Hashmap

#include <ef/hashmap.h>
#include <ef/thread.h>


class EF_ThreadSafePtr2PtrHashmap : public EF_Ptr2PtrHashmap
{
public:
	EF_ThreadSafePtr2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount);
	virtual EFBool Get (void* key, void** pValue);
	virtual void Set (void* key, void* value);
	virtual void Remove (void* key);
	virtual void RemoveAll (void);
	const EF_Thread::Mutex& GetMutex();
private:
	EF_Thread::Mutex m_Mutex;

public:
	//给EF_ThreadSafePtr2PtrHashmap提供访问iterator.GetKey()的权限
	class iterator : public EF_HashKV_Iterator
	{
	protected:
		EFBool GetKey(void** ppkey) { return EF_HashKV_Iterator::GetKey(ppkey); }
	friend class EF_ThreadSafePtr2PtrHashmap; 
	};

	iterator begin() { return *(iterator*)_begin(); }

protected:
	virtual EF_HashKV_Iterator* CreateIterator() { return new iterator; }
};


///////////////////////////////////////////////////////////////////////////////
// EF_ThreadSafeText2PtrHashmap: 键为文本(EFChar*)、值为指针(void*)的哈希表

class EF_ThreadSafeText2PtrHashmap : public EF_ThreadSafePtr2PtrHashmap
{
public:
	EF_ThreadSafeText2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount, EFBool blNeedFreeKey = EF_FALSE) 
		: EF_ThreadSafePtr2PtrHashmap (nPresumedElementCount)
	{
		SetFunctions (EF_hashkv_GetTextHashValue, EF_hashkv_IsTextEquals);
		SetIsNeedFreeKey(blNeedFreeKey);
	}

	EFBool Get (EFChar* key, void** pValue)
	{
		return EF_ThreadSafePtr2PtrHashmap::Get ((void*)key, pValue);
	}
	void Set (EFChar* key, void* value)
	{
		EF_ThreadSafePtr2PtrHashmap::Set ((void*)key, value);
	}
	void Remove (EFChar* key)
	{
		EF_ThreadSafePtr2PtrHashmap::Remove ((void*)key);
	}

};

//////////////////////////////////////////////////////////////////////////
// EF_ThreadSafeInt2PtrHashmap: 键为整数(int)、值为指针(void*)的哈希表

class EF_ThreadSafeInt2PtrHashmap : public EF_ThreadSafePtr2PtrHashmap
{
public:
	EF_ThreadSafeInt2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount) 
		: EF_ThreadSafePtr2PtrHashmap (nPresumedElementCount)
	{
	}

	EFBool Get (int key, void** pValue)
	{
		return EF_ThreadSafePtr2PtrHashmap::Get ((void*)key, pValue);
	}
	void Set (int key, void* value)
	{
		EF_ThreadSafePtr2PtrHashmap::Set ((void*)key, value);
	}
	void Remove (int key)
	{
		EF_ThreadSafePtr2PtrHashmap::Remove ((void*)key);
	}

};

//////////////////////////////////////////////////////////////////////////
// EF_ThreadSafeInt2IntHashmap: 键和值均为整数(int)的哈希表

class EF_ThreadSafeInt2IntHashmap : public EF_ThreadSafePtr2PtrHashmap
{
public:
	EF_ThreadSafeInt2IntHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount)
		: EF_ThreadSafePtr2PtrHashmap (nPresumedElementCount)
	{
	}

	EFBool Get (int key, int* pValue)
	{
		return EF_ThreadSafePtr2PtrHashmap::Get ((void*)key, (void**)pValue);
	}
	void Set (int key, int value)
	{
		EF_ThreadSafePtr2PtrHashmap::Set ((void*)key, (void*)value);
	}
	void Remove (int key)
	{
		EF_ThreadSafePtr2PtrHashmap::Remove ((void*)key);
	}
};


#endif //__EF_TS_HASH_MAP_H__
