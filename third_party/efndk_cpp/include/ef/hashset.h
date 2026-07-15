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


#ifndef __EF_HASH_SET_H__
#define __EF_HASH_SET_H__

#include <ef/types.h>
#include <ef/hashkv.h>

// 哈希集合
class EF_PtrHashset : public EF_HashKV
{
public:
	EF_PtrHashset (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount)
		: EF_HashKV (nPresumedElementCount)
	{
		EF_HashKV::SetMemCellSize (sizeof (Hashset_KV));
	}

	EFBool IsExist (void* value)
	{
		return EF_HashKV::IsExist (value);
	}

	void Set (void* value)
	{
		EF_HashKV::Set (value, NULL);
	}

	EFBool Remove (void* value)
	{
		return EF_HashKV::Remove (value);
	}

	struct Hashset_KV : public EF_KV
	{
		void* value;  // 此值是被当作key存储在EF_HashKV中的(只有key可被比较)! 切记!
	};

	virtual void* KV_GetKey (EF_KV* pKV) const
	{ 
		ASSERT (pKV);
		return ((Hashset_KV*)pKV)->value;
	}
	virtual void* KV_GetValue (EF_KV* pKV) const
	{
		return NULL; //no value to return
	}
	virtual void KV_SetKey (EF_KV* pKV, void* key)
	{
		ASSERT (pKV);
		((Hashset_KV*)pKV)->value = key; 
	}
	virtual void KV_SetValue (EF_KV* pKV, void* value)
	{
		// no value to set
	}

public:
	class ps_iterator : public EF_HashKV_Iterator
	{
	public:
		EFBool GetValue (void** ppValue)
        {
            return EF_HashKV_Iterator::GetKey (ppValue); /*yes, GetKey()*/
        }
	};

	ps_iterator begin ()
    {
        return *(ps_iterator*)_begin();
    }

protected:
	virtual EF_HashKV_Iterator* CreateIterator ()
    {
        return new ps_iterator;
    }
};

//////////////////////////////////////////////////////////////////////////

class EF_TextHashset : public EF_PtrHashset
{
public:
	EF_TextHashset (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount, EFBool blNeedFreeValue = EF_FALSE) 
		: EF_PtrHashset (nPresumedElementCount)
	{
		EF_HashKV::SetFunctions (EF_hashkv_GetTextHashValue, EF_hashkv_IsTextEquals);
		EF_HashKV::SetIsNeedFreeKey (blNeedFreeValue);
	}

	EFBool IsExist (EFChar* value)
	{
		return EF_PtrHashset::IsExist ((void*)value);
	}
	void Set (EFChar* value)
	{
		EF_PtrHashset::Set ((void*)value);
	}
	EFBool Remove (EFChar* value)
	{
		return EF_PtrHashset::Remove ((void*)value);
	}
};

//////////////////////////////////////////////////////////////////////////

class EF_IntHashset : public EF_PtrHashset
{
public:
	EF_IntHashset(int nPresumedElementCount = Hashkv_DefaultPresumedElementCount) 
		: EF_PtrHashset (nPresumedElementCount)
	{
	}

	EFBool IsExist (int value)
	{
		return EF_PtrHashset::IsExist ((void*)value);
	}
	void Set (int value)
	{
		EF_PtrHashset::Set ((void*)value);
	}
	EFBool Remove (int value)
	{
		return EF_PtrHashset::Remove ((void*)value); 
	}
};

#endif //__EF_HASH_SET_H__
