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


//hashmap.h
#ifndef __EF_HASH_MAP_H__
#define __EF_HASH_MAP_H__

#include <ef/hashkv.h>

/*
  此文件中实现了以下几种哈希表：
    EF_Ptr2PtrHashmap:  键和值都是指针(void*)的哈希表
    EF_Text2PtrHashmap: 键为文本(EFChar*)、值为指针(void*)的哈希表
    EF_Int2PtrHashmap:  键为整数(int)、值为指针(void*)的哈希表
    EF_Int2IntHashmap:  键和值均为整数(int)的哈希表
*/

//////////////////////////////////////////////////////////////////////////
// EF_Ptr2PtrHashmap: 键和值都是指针(void*)的哈希表

class EF_Ptr2PtrHashmap : public EF_HashKV
{
public:
	EF_Ptr2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount)
		: EF_HashKV (nPresumedElementCount)
	{
	}

	EFBool Get (void* key, void** ppValue = NULL)
	{
		return EF_HashKV::Get (key, ppValue);
	}
	void Set (void* key, void* value)
	{
		EF_HashKV::Set (key, value);
	}
	EFBool Remove (void* key)
	{
		return EF_HashKV::Remove (key);
	}

public:
	class p2p_iterator : public EF_HashKV_Iterator
	{
	public:
		EFBool GetKey (void** ppKey)  { return EF_HashKV_Iterator::GetKey(ppKey); }
		EFBool GetValue (void** ppValue)  { return EF_HashKV_Iterator::GetValue(ppValue); }
		EFBool GetKeyValue (void** ppKey, void** ppValue)  { return EF_HashKV_Iterator::GetKeyValue(ppKey, ppValue); }
	};

	p2p_iterator begin () { return *(p2p_iterator*)_begin (); }

protected:
	virtual EF_HashKV_Iterator* CreateIterator()  { return new p2p_iterator; }
};


///////////////////////////////////////////////////////////////////////////////
// EF_Text2PtrHashmap: 键为文本(EFChar*)、值为指针(void*)的哈希表

class EF_Text2PtrHashmap : public EF_Ptr2PtrHashmap
{
public:
	EF_Text2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount, EFBool blNeedFreeKey = EF_FALSE) 
		: EF_Ptr2PtrHashmap (nPresumedElementCount)
	{
		EF_HashKV::SetFunctions (EF_hashkv_GetTextHashValue, EF_hashkv_IsTextEquals);
		EF_HashKV::SetIsNeedFreeKey (blNeedFreeKey);
	}

	EFBool Get (EFChar* key, void** pValue = NULL)
	{
		return EF_Ptr2PtrHashmap::Get ((void*)key, pValue);
	}
	void Set (EFChar* key, void* value)
	{
		EF_Ptr2PtrHashmap::Set ((void*)key, value);
	}
	EFBool Remove (EFChar* key)
	{
		return EF_Ptr2PtrHashmap::Remove ((void*)key);
	}
};

//////////////////////////////////////////////////////////////////////////
// EF_Int2PtrHashmap: 键为整数(int)、值为指针(void*)的哈希表

class EF_Int2PtrHashmap : public EF_Ptr2PtrHashmap
{
public:
	EF_Int2PtrHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount) 
		: EF_Ptr2PtrHashmap (nPresumedElementCount)
	{
	}

	EFBool Get (int key, void** pValue = NULL)
	{
		return EF_Ptr2PtrHashmap::Get ((void*)key, pValue);
	}
	void Set (int key, void* value)
	{
		EF_Ptr2PtrHashmap::Set ((void*)key, value);
	}
	EFBool Remove (int key)
	{
		return EF_Ptr2PtrHashmap::Remove ((void*)key);
	}
};

//////////////////////////////////////////////////////////////////////////
// EF_Int2IntHashmap: 键和值均为整数(int)的哈希表

class EF_Int2IntHashmap : public EF_Ptr2PtrHashmap
{
public:
	EF_Int2IntHashmap (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount)
		: EF_Ptr2PtrHashmap (nPresumedElementCount)
	{
	}

	EFBool Get (int key, int* pValue = NULL)
	{
		return EF_Ptr2PtrHashmap::Get ((void*)key, (void**)pValue);
	}
	void Set (int key, int value)
	{
		EF_Ptr2PtrHashmap::Set ((void*)key, (void*)value);
	}
	EFBool Remove (int key)
	{
		return EF_Ptr2PtrHashmap::Remove ((void*)key);
	}
};

#endif //__EF_HASH_MAP_H__
