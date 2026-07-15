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


#ifndef __EF_HASH_KV_H__
#define __EF_HASH_KV_H__

#include <ef/types.h>
#include <ef/functions.h>
#include <ef/classes.h>

// EF_HashKV类专用的内存池, 每次只能分配指定大小的内存(大小在构造函数中指定)
class EF_HashKVMemoryPool
{
protected:
	EF_HashKVMemoryPool (int nMemCellSize);
	~EF_HashKVMemoryPool ();

	// 分配一块可用内存（此内存块大小为nMemCellSize）
	void* Malloc ();
	// 释放指定内存
	void Free (void* pMemory);
	// 释放所有从本内存池分配的内存
	void FreeAll ();
	// 重新设置MemCellSize. 本操作将导致释放内存池中所有现有内存!
	void SetMemCellSize (int nMemCellSize);

private:
	int m_nMemCellSize;
	int m_nCurrentCellIndex;
    EF_PtrArray m_paryMemBlocks, m_paryFreedCells;

    friend class EF_HashKV;
};

//////////////////////////////////////////////////////////////////////////////////

typedef EFDWord (*EF_PFN_HashKV_GetHashValue) (void* key);
typedef EFBool  (*EF_PFN_HashKV_IsKeyEquals)  (void* key1, void* key2);

// 本类是Hashmap,Hashset等类的基础类

/*
使用此类, 通常要做的工作:
1. 从EF_HashKV继承出自己的子类
2. 添加相应的Set/Get方法
3. 提供自己的iterator(继承自EF_HashKV::iterator), 并为iterator添加Get方法; 同时覆盖EF_HashKV::CreateIterator
   添加begin()方法, 返回自已的iterator:iterator begin() { return *(iterator*)_begin(); }
4. [可选]提供自己的EF_KV(继承自EF_HashKV::EF_KV), 同时覆盖EF_HashKV的CreateKV/DeleteKV/KV_GetKey/KV_GetValue/KV_SetKey/KV_SetValue等方法
   调用SetMemCellSize (sizeof(your_KV))重新初始化内存单元尺寸, 以保证CreateKV()中从内存池分配合适大小的内存
*/

// 提供基本的EF_KV接口, 并不存储实际数据. 至于如何存放数据, 由其子类决定. EF_HashKV提供EF_KV的一个默认实现HashKV_KV.
// 如果EF_HashKV需要使用自己的EF_KV, 需继承自EF_KV得到新的结构, 同时需覆盖EF_HashKV的以下虚方法: CreateKV/DeleteKV/KV_GetKey/KV_GetValue/KV_SetKey/KV_SetValue
// 需保证EF_KV及其子类均是struct而非class, 以便应用EF_HashKVMemoryPool.
struct EF_KV
{
	EF_KV* pNext;
};

class EF_HashKV;
class EF_HashKV_Iterator
{
public:
	EF_HashKV_Iterator ();
	EF_HashKV_Iterator (EF_HashKV* pOwner);
	EF_HashKV_Iterator (const EF_HashKV_Iterator& other);
	void CopyFrom (const EF_HashKV_Iterator& other);
	EF_HashKV_Iterator& operator= (const EF_HashKV_Iterator& other);
	bool operator == (EF_HashKV_Iterator& other);
	bool operator != (EF_HashKV_Iterator& other);
	void skip ();

protected:
	// 并非所有子类都需要这几个方法(即使需要也可能含义不同), 故设置为protected.
	// EF_HashKV的子类应提供iterator子类, 公开新的GetKey/Value等方法, 并覆盖EF_HashKV的CreateIterator ().
	EFBool GetKey (void** ppKey);
	EFBool GetValue (void** ppValue);
	EFBool GetKeyValue (void** ppKey, void** ppValue);

protected:
	void reset ();

	EFDWord m_row;
	EF_KV* m_pKV;
	EF_HashKV* m_pOwner;

    friend class EF_HashKV;
};

class EF_HashKV
{
protected:
	struct HashKV_KV;

public:
	#define Hashkv_DefaultPresumedElementCount 1024 // 默认的初始成员数
	// 参数nPresumedElementCount用于指定估计的元素个数。
	// 此值较小，键位重复的可能性较大，效率较低；此值较大，则键位重复的可能性较小，效率较高，但占用空间较大。
	// 此值不对哈希表中的容量造成影响，即，此哈希表总是可以存放任意个数的key-value对。
	EF_HashKV (int nPresumedElementCount = Hashkv_DefaultPresumedElementCount);
	virtual ~EF_HashKV ();

	void RemoveAll ();
	int GetCount ();
	EFBool IsExist (void* key);
	
#ifdef _DEBUG
	void Dump ();
#endif

protected:
	// 并非所有子类都需要这几个方法, 故设置为protected. 如果子类需要, 需手工添加.
	EFBool Get (void* key, void** pValue = NULL);
	void Set (void* key, void* value);
	EFBool Remove (void* key);
	void SetIsNeedFreeKey (EFBool blNeedFreeKey);
	void SetFunctions (EF_PFN_HashKV_GetHashValue pfn_GetKeyHashValue, EF_PFN_HashKV_IsKeyEquals pfn_IsKeyEquals);
	// 重新设置MemCellSize. 本操作将导致释放内存池中所有现有内存!
	void SetMemCellSize (int nMemCellSize)
    {
        m_KVPool.SetMemCellSize (nMemCellSize);
    }

protected:
	// EF_HashKV子类应提供新的EF_HashKV_Iterator的子类, 并覆盖本方法
	virtual EF_HashKV_Iterator* CreateIterator ()
    {
        return new EF_HashKV_Iterator ();
    } 

	EF_HashKV_Iterator* _begin ();
	EF_KV* GetKVs (void* key, int* pnHash = NULL) const;
	EF_KV* GetKV (void* key) const;
	void InitHashTable (int nPresumedElementCount);

private:
	EF_HashKVMemoryPool m_KVPool;

protected:
	EFBool m_blNeedFreeKey;
	EF_KV** m_pHashTable;
	int m_nCount;
	int m_nHashTableSize;
	EF_HashKV_Iterator* m_pIterator;
	EF_PFN_HashKV_GetHashValue m_pfn_GetKeyHashValue;
	EF_PFN_HashKV_IsKeyEquals m_pfn_IsKeyEquals;

	void KV_AppendKV (EF_KV* pKV, EF_KV* pNewKV)
	{
		EF_KV* pTempKV = pKV;
		while (pTempKV && pTempKV->pNext)
			pTempKV = pTempKV->pNext;
		pTempKV->pNext = pNewKV;
	}

	// HashKV_KV为EF_KV默认实现, key,value均为void*
	struct HashKV_KV : public EF_KV
	{
		void* key;
		void* value;
	};

	EF_KV* CreateKV ()
	{
		return (HashKV_KV*)m_KVPool.Malloc ();
	}
	void DeleteKV (EF_KV* pKV)
	{ 
		m_KVPool.Free (pKV);
	}

public:
	virtual void* KV_GetKey (EF_KV* pKV) const
	{
		ASSERT(pKV);
		return ((HashKV_KV*)pKV)->key;
	}
	virtual void* KV_GetValue (EF_KV* pKV) const
	{
		ASSERT (pKV);
		return ((HashKV_KV*)pKV)->value; 
	}
	virtual void KV_SetKey (EF_KV* pKV, void* key)
	{
		ASSERT (pKV);
		((HashKV_KV*)pKV)->key = key; 
	}
	virtual void KV_SetValue (EF_KV* pKV, void* value)
	{
		ASSERT (pKV);
		((HashKV_KV*)pKV)->value = value;
	}

    friend class EF_HashKV_Iterator;
};


///////////////////////////////////////////////////////////////////////////////

EFDWord EF_hashkv_GetIntHashValue (void* key);
EFBool  EF_hashkv_GetIntHashValue (void* key1, void* key2);
EFDWord EF_hashkv_GetTextHashValue (void* key);
EFBool  EF_hashkv_IsTextEquals (void* key1, void* key2);


#endif //__EF_HASH_KV_H__
