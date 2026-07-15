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


//hashkv.cpp

#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include <ef/hashkv.h>

//////////////////////////////////////////////////////////////////////////
// EF_HashKV

EF_HashKV::EF_HashKV (int nPresumedElementCount) : m_KVPool (sizeof (HashKV_KV))
{
	m_nCount = 0;
	m_pHashTable = NULL;
	m_nHashTableSize = 0;

	m_pIterator = NULL;  // CreateIterator (); 不在构造函数中创建virtual方法，推迟到_begin()中

	SetIsNeedFreeKey (EF_FALSE);
	SetFunctions (EF_hashkv_GetIntHashValue, EF_hashkv_GetIntHashValue);
	InitHashTable (nPresumedElementCount);
}

EF_HashKV::~EF_HashKV ()
{
	RemoveAll ();
	delete[] m_pHashTable;
	delete m_pIterator;
}

void EF_HashKV::SetIsNeedFreeKey (EFBool blNeedFreeKey)
{
	m_blNeedFreeKey = blNeedFreeKey;
}

void EF_HashKV::SetFunctions (EF_PFN_HashKV_GetHashValue pfn_GetKeyHashValue, EF_PFN_HashKV_IsKeyEquals pfn_IsKeyEquals)
{
    ASSERT (pfn_GetKeyHashValue != NULL && pfn_IsKeyEquals != NULL);

    m_pfn_GetKeyHashValue = pfn_GetKeyHashValue;
	m_pfn_IsKeyEquals = pfn_IsKeyEquals;
}

void EF_HashKV::InitHashTable (int nPresumedElementCount)
{
	RemoveAll ();

	ASSERT (nPresumedElementCount > 0);
	if (nPresumedElementCount <= 0)
        nPresumedElementCount = Hashkv_DefaultPresumedElementCount;

	int nHashSize = (int)(nPresumedElementCount * 3 * 0.618);
	static int s_nPrimeNums [] =
    {
        257, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, 131071, 262007, 525013, 
	    1050239, 2100001, 4201123, 8402357, 16805323, 33610001, 67220089, INT_MAX
    };

    int i = 0;
	while (s_nPrimeNums [i] < nHashSize)
        i++;

    nHashSize = s_nPrimeNums [i];

	if (m_pHashTable)
        delete[] m_pHashTable;
	m_pHashTable = new EF_KV* [nHashSize];
	memset (m_pHashTable, 0, sizeof (EF_KV*) * nHashSize);

	m_nHashTableSize = nHashSize;
}

void EF_HashKV::RemoveAll ()
{
	if (m_pHashTable)
	{
		EF_KV *pKV, *pTempKV;
		for (int i = 0; i < m_nHashTableSize; i++)
		{
			pKV = m_pHashTable [i];
			while (pKV)
			{
				if (m_blNeedFreeKey)
					EF_free (KV_GetKey (pKV));
				pTempKV = pKV;
				pKV = pKV->pNext;
				//DeleteKV (pTempKV); //don't need free it, because of the following m_KVPool.FreeAll()
			}
		}

		memset (m_pHashTable, 0, sizeof (EF_KV*) * m_nHashTableSize);
	}

	m_nCount = 0;
	m_KVPool.FreeAll ();
}

EFBool EF_HashKV::Remove (void* key)
{
	int nHash;
	EF_KV* pKVs = GetKVs (key, &nHash);
	if (pKVs == NULL)
        return EF_FALSE;  // not found

    if (m_pfn_IsKeyEquals (key, KV_GetKey (pKVs)))
    {
        EF_KV* pTempKV = pKVs;
        if (m_blNeedFreeKey)
            EF_free (KV_GetKey (pKVs));
		m_pHashTable [nHash] = pKVs->pNext;
        DeleteKV (pTempKV);
        m_nCount--;
        return EF_TRUE;
    }

	EF_KV* pKV = pKVs->pNext; EF_KV* pPrevKV = pKVs;
	while (pKV)
	{
		if (m_pfn_IsKeyEquals (key, KV_GetKey (pKV)))
		{
			EF_KV* pTempKV  = pKV;
			pPrevKV->pNext = pKV->pNext;

			if (m_blNeedFreeKey)
				EF_free (KV_GetKey (pTempKV));
			DeleteKV (pTempKV);
			m_nCount--;
			return EF_TRUE;
		}

		pPrevKV = pKV;
		pKV = pKV->pNext;
	}

    return EF_FALSE;
}

void EF_HashKV::Set (void* key, void* value)
{
	if (m_pHashTable == NULL)
		InitHashTable (m_nHashTableSize);

	EF_KV* pKV = GetKV (key);
	if (pKV)
	{
		if (m_blNeedFreeKey)
			EF_free (KV_GetKey (pKV));
		KV_SetKey (pKV, key);
		KV_SetValue (pKV, value);
	}
	else
	{
		EF_KV* pNewKV = CreateKV ();
		KV_SetKey (pNewKV, key);
		KV_SetValue (pNewKV, value);
		pNewKV->pNext = NULL;

		int nHash;
		EF_KV* pKVs = GetKVs (key, &nHash);
		if (pKVs)
			KV_AppendKV (pKVs, pNewKV);
		else
			m_pHashTable [nHash] = pNewKV;

		m_nCount++;
	}
}

EFBool EF_HashKV::IsExist (void* key)
{
    return GetKV (key) != NULL ? EF_TRUE : EF_FALSE;
}

EFBool EF_HashKV::Get (void* key, void** ppValue)
{
	EF_KV* pKV = GetKV (key);
	if (pKV)
	{
		if (ppValue)
            *ppValue = KV_GetValue (pKV);
		return EF_TRUE;
	}
	else
	{
		if (ppValue)
            *ppValue = NULL;
		return EF_FALSE;
	}
}

EF_KV* EF_HashKV::GetKVs (void* key, int* pnHash) const
{
	int nHash = m_pfn_GetKeyHashValue (key) % m_nHashTableSize; 
	if (pnHash)
		*pnHash = nHash;

	if (m_pHashTable == NULL || m_nCount == 0)
        return NULL;
    else
    	return m_pHashTable [nHash];
}

EF_KV* EF_HashKV::GetKV (void* key) const
{
	for (EF_KV* pKV = GetKVs (key); pKV != NULL; pKV = pKV->pNext)
	{
		if (m_pfn_IsKeyEquals (KV_GetKey (pKV), key))
			return pKV;
	}
	return NULL;
}

int EF_HashKV::GetCount ()
{
	return m_nCount;
}

#ifdef _DEBUG

//用于查看键位分布, 以及重复情况
void EF_HashKV::Dump ()
{
	printf ("\nBegin Dump of Hashmap [%d] \n", (EFDWord)this);
	printf ("--------------------------------------------------------------\n");
	if (m_pHashTable == NULL)
	{
		printf ("Not init yet. Hash Table Size: %d. \n", m_nHashTableSize);
	}
	else
	{
		printf ("Hash Table Size: %d \n", m_nHashTableSize);
		printf ("Elements' Count: %d \n\n", m_nCount);
		EF_KV *pKV;
		for (int i = 0; i < m_nHashTableSize; i++)
		{
			pKV = m_pHashTable [i];
			int n = 0; 
			while (pKV) 
			{
				n++;
				pKV = pKV->pNext;
			}

			static int s_PrintedDotCountInThisLine = 0;
			if(n == 0)
			{
				printf (".");
				if (++s_PrintedDotCountInThisLine >= 100)
				{
					printf ("\n");
					s_PrintedDotCountInThisLine = 0;
				}
			}
			else
			{
				s_PrintedDotCountInThisLine = 0;
				printf ("\nIndex: %d, Count: %d  ", i, n);
			}

			pKV = m_pHashTable [i];
			while (pKV)
			{
				printf ("([%d]-[%d])", (EFDWord)(KV_GetKey (pKV)), (EFDWord)(KV_GetValue (pKV)));
				pKV = pKV->pNext;
				printf (pKV ? " -> " : "\n");
			}
		}
	}

	printf ("--------------------------------------------------------------\n");
	printf ("End Dump of Hashmap [%d] \n\n\n", (EFDWord)this);
}

#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////
// EF_HashKV_Iterator

EF_HashKV_Iterator::EF_HashKV_Iterator ()
{
	reset ();
	m_pOwner = NULL;
}

EF_HashKV_Iterator::EF_HashKV_Iterator (EF_HashKV* pOwner)
{
	reset ();
	m_pOwner = pOwner;
}

EF_HashKV_Iterator::EF_HashKV_Iterator (const EF_HashKV_Iterator& other)
{
	CopyFrom (other);
}

void EF_HashKV_Iterator::reset ()
{
	m_row = 0;
	m_pKV = NULL;
}

void EF_HashKV_Iterator::CopyFrom (const EF_HashKV_Iterator& other)
{
	m_pOwner = other.m_pOwner;
	m_row = other.m_row;
	m_pKV = other.m_pKV;
}

EF_HashKV_Iterator& EF_HashKV_Iterator::operator= (const EF_HashKV_Iterator& other)
{
	CopyFrom (other);
	return *this;
}

bool EF_HashKV_Iterator::operator== (EF_HashKV_Iterator& other)
{
	return (m_pOwner==other.m_pOwner && m_pKV==other.m_pKV && m_row==other.m_row);
}

bool EF_HashKV_Iterator::operator!= (EF_HashKV_Iterator& other)
{
	return (m_pOwner!=other.m_pOwner || m_pKV!=other.m_pKV || m_row!=other.m_row);
}

EF_HashKV_Iterator* EF_HashKV::_begin ()
{
	if (m_pIterator == NULL)
		m_pIterator = CreateIterator ();

	m_pIterator->reset ();
	m_pIterator->m_pOwner = this;  //!

	if (GetCount () > 0)
	{
		for (int i = 0; i < m_nHashTableSize; i++)
		{
			EF_KV* pKV = m_pHashTable [i];
			if (pKV)
			{
				m_pIterator->m_row = i;
				m_pIterator->m_pKV = pKV;
				break;
			}
		}
	}
	return m_pIterator;
}

void EF_HashKV_Iterator::skip ()
{
    if (m_pKV == NULL)
        return;

    if (m_pKV->pNext)
    {
        m_pKV = m_pKV->pNext;
    }
    else
    {
        ASSERT (m_pOwner != NULL);
        for (int i = m_row + 1; i < m_pOwner->m_nHashTableSize; i++)
        {
            EF_KV* pKV = m_pOwner->m_pHashTable [i];
            if (pKV)
			{
                m_row = i;
                m_pKV = pKV; 
                return;
            }
        }

        m_pKV = NULL;
    }
}

EFBool EF_HashKV_Iterator::GetKey (void** ppKey)
{
	if (m_pKV == NULL)
		return EF_FALSE;

	if (ppKey)
		*ppKey = m_pOwner->KV_GetKey (m_pKV);
	return EF_TRUE;
}

EFBool EF_HashKV_Iterator::GetValue (void** ppValue)
{
	if (m_pKV == NULL)
		return EF_FALSE;

	if (ppValue)
		*ppValue = m_pOwner->KV_GetValue (m_pKV);
	return EF_TRUE;
}

EFBool EF_HashKV_Iterator::GetKeyValue (void** ppKey, void** ppValue)
{
	if (m_pKV == NULL)
		return EF_FALSE;

	if (ppKey)
		*ppKey = m_pOwner->KV_GetKey (m_pKV);
	if (ppValue)
		*ppValue = m_pOwner->KV_GetValue (m_pKV);

	return EF_TRUE;
}

//////////////////////////////////////////////////////////////////////////

EFDWord EF_hashkv_GetIntHashValue (void* key)
{
	return (EFDWord)key;
}

EFBool EF_hashkv_GetIntHashValue (void* key1, void* key2)
{
	return ((intptr_t)key1 == (intptr_t)key2);
}


EFDWord EF_hashkv_GetTextHashValue (void* key)
{
	return EF_GetStrHash ((EFChar*)key);
}

EFBool EF_hashkv_IsTextEquals (void* key1, void* key2)
{
    return (EF_strcmp ((EFChar*)key1, (EFChar*)key2) == 0 ? EF_TRUE : EF_FALSE);
}

//////////////////////////////////////////////////////////////////////////

#define MP_CELL_COUNT_PRE_BLOCK  100

// 在内存池内部, 由多个Block组成表, 每个Block中有固定个数的Cell. 以Block为单位进行实际的内存分配.
EF_HashKVMemoryPool::EF_HashKVMemoryPool (int nMemCellSize)
{
	ASSERT (nMemCellSize > 0);
	m_nMemCellSize = nMemCellSize;
	m_nCurrentCellIndex = -1;

    // 设置用作记录当前空闲内存块数组的内存分配对齐尺寸为1K，以提高操作效率。
    m_paryFreedCells.SetMemAlignSize (1024);
}

EF_HashKVMemoryPool::~EF_HashKVMemoryPool ()
{
	FreeAll ();
}

void* EF_HashKVMemoryPool::Malloc ()
{
	// 如果存在已释放的Cell, 选择其中一个返回
	int nMaxIndex = m_paryFreedCells.GetUpperBound ();
	if (nMaxIndex >= 0)
	{
        // 从最后删除效率最高
		void* pMemory = m_paryFreedCells [nMaxIndex];
		m_paryFreedCells.Realloc (nMaxIndex * sizeof (void*));
		return pMemory;
	}

	// 如果当前Block尾部还有可用空间, 返回之
    int nBlockIndex = m_paryMemBlocks.GetUpperBound ();
	if (nBlockIndex >= 0 && m_nCurrentCellIndex < MP_CELL_COUNT_PRE_BLOCK - 1)
	{
		m_nCurrentCellIndex++;
        ASSERT (m_nCurrentCellIndex >= 0);
    	return (EFByte*)m_paryMemBlocks [nBlockIndex] + m_nCurrentCellIndex * m_nMemCellSize;
	}

	// 分配新的Block, 并返回其首地址
	void* pNewBlock = EF_malloc (m_nMemCellSize * MP_CELL_COUNT_PRE_BLOCK);
	m_paryMemBlocks.Add (pNewBlock);
	m_nCurrentCellIndex = 0;
	return pNewBlock;
}

void EF_HashKVMemoryPool::Free (void* pMemory)
{
	if (pMemory != NULL)
    {
    #ifdef _DEBUG
        // 所释放内存必须是以前从内存池中分配的
        bool blFound = false;
        int nBlockSize = m_nMemCellSize * MP_CELL_COUNT_PRE_BLOCK;
	    int nBlocksCount = m_paryMemBlocks.GetCount ();
	    for (int i = 0; i < nBlocksCount; i++)
        {
            void* pBlock = m_paryMemBlocks [i];
            if (pMemory >= pBlock && pMemory < (EFByte*)pBlock + nBlockSize)
                blFound = true;
        }
        ASSERT (blFound);
    #endif

	    m_paryFreedCells.Add (pMemory);  // 记录已释放的地址, 以备重用
    }
}

void EF_HashKVMemoryPool::FreeAll ()
{
	// 释放所有Block内存
	int nBlocksCount = m_paryMemBlocks.GetCount ();
	for (int i = 0; i < nBlocksCount; i++)
		EF_free (m_paryMemBlocks [i]);

    m_paryMemBlocks.RemoveAll ();
    m_paryFreedCells.RemoveAll ();
	m_nCurrentCellIndex = -1;
}

void EF_HashKVMemoryPool::SetMemCellSize (int nMemCellSize)
{
	ASSERT (nMemCellSize > 0);

    FreeAll ();
	m_nMemCellSize = nMemCellSize;
}
