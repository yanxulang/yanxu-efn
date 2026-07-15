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


// ecl_classes.cpp

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <stdarg.h>

#include <ef/ef.h>
#include <ef/classes.h>
#include <ef/functions.h>

#ifdef _DEBUG
    #define MCHECK_MEM_POINTER(p)  \
        if ((EFByte*)(p) >= GetPtr () && (EFByte*)(p) < GetPtr () + GetSize ())  \
            FAIL;
#else
    #define MCHECK_MEM_POINTER(p)
#endif

EFByte* EF_MiniMem::Alloc (int nSize)
{
	if (m_pData != NULL)
		return Realloc (nSize);

	if (nSize == 0)
		return NULL;

    int nAlignSize = GetMemAlignSize ();
    ASSERT (nAlignSize >= 0);
    int nAllocSize = nSize + nAlignSize - (nSize % nAlignSize);

	m_pData = (EFByte*)EF_malloc (nAllocSize);
	if (m_pData == NULL)
	{
		m_nAllocedSize = 0;
		m_nSize = 0;
	}
	else
	{
		m_nAllocedSize = nAllocSize;
		m_nSize = nSize;
	}
	return m_pData;
}

EFByte* EF_MiniMem::Realloc (int nSize)
{
	if (nSize == 0)
	{
		Free ();
		return NULL;
	}
	if (m_pData == NULL)
		return Alloc (nSize);

    int nAlignSize = GetMemAlignSize ();
    ASSERT (nAlignSize >= 0);
	if (m_nAllocedSize >= nSize && m_nAllocedSize - nSize <= nAlignSize)
	{
		m_nSize = nSize;
		return m_pData;
	}

    int nAllocSize;
    if (nAlignSize == 0)
        nAllocSize = nSize;
    else
        nAllocSize = nSize + nAlignSize - (nSize % nAlignSize);
    ASSERT (nAllocSize >= nSize);

	EFByte* pNew = (EFByte*)EF_realloc (m_pData, nAllocSize);
	if (pNew != NULL)
	{
		m_pData = pNew;
		m_nAllocedSize = nAllocSize;
		m_nSize = nSize;
		return m_pData;
	}
	else
		return NULL;
}

void EF_MiniMem::Free ()
{
    if (m_pData != NULL)
    	EF_free (m_pData);

	m_pData = NULL;
	m_nAllocedSize = m_nSize = 0;
}

void EF_MiniMem::xchg  (EF_MiniMem& mem)
{
    MCHECK_MEM_POINTER (mem.GetPtr ())

	EFByte* p = m_pData;
	m_pData = mem.m_pData;
	mem.m_pData = p;

	int n = m_nAllocedSize;
	m_nAllocedSize = mem.m_nAllocedSize;
	mem.m_nAllocedSize = n;

	n = m_nSize;
	m_nSize = mem.m_nSize;
	mem.m_nSize = n;
}

void EF_MiniMem::MoveFrom (EF_MiniMem& mem)
{
    MCHECK_MEM_POINTER (mem.GetPtr ())

    Free ();

    m_pData = mem.m_pData;
	m_nAllocedSize = mem.m_nAllocedSize;
	m_nSize = mem.m_nSize;

	mem.m_pData = NULL;
	mem.m_nAllocedSize = mem.m_nSize = 0;
}

void EF_MiniMem::Attach (void* pData, int nSize)
{
    MCHECK_MEM_POINTER (pData)
    ASSERT (nSize >= 0);

    Free ();

	m_pData = (EFByte*)pData;
	m_nAllocedSize = m_nSize = nSize;
}

EFByte* EF_MiniMem::Detach (int* pnSize)
{
    ASSERT (pnSize != NULL);
    *pnSize = m_nAllocedSize;
    return Detach ();
}

EFByte* EF_MiniMem::Detach ()
{
	if (m_nSize == 0)
		return NULL;

	EFByte* pData = m_pData;

	m_pData = NULL;
	m_nAllocedSize = m_nSize = 0;
	return pData;
}

///////////////////////////////////////////

void EF_MiniMem::AddDouble (EFDouble db)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFDouble))
	{
		*(EFDouble*)(m_pData + m_nSize) = db;
		m_nSize += sizeof (EFDouble);
	}
	else
		Append (&db, sizeof (EFDouble));
}

void EF_MiniMem::AddDWord (EFDWord dw)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFDWord))
	{
		*(EFDWord*)(m_pData + m_nSize) = dw;
		m_nSize += sizeof (EFDWord);
	}
	else
		Append (&dw, sizeof (EFDWord));
}

void EF_MiniMem::AddWord (EFWord w)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFWord))
	{
		*(EFWord*)(m_pData + m_nSize) = w;
		m_nSize += sizeof (EFWord);
	}
	else
		Append (&w, sizeof (EFWord));
}

void EF_MiniMem::AddInt (EFInt n)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFInt))
	{
		*(EFInt*)(m_pData + m_nSize) = n;
		m_nSize += sizeof (EFInt);
	}
	else
		Append (&n, sizeof (EFInt));
}

void EF_MiniMem::AddByte (EFByte bt)
{
	if (m_nAllocedSize > m_nSize)
		m_pData [m_nSize++] = bt;
	else
		Append (&bt, sizeof (bt));
}

void EF_MiniMem::AddSpecNumBytes (int nBytesCount, ...)
{
	va_list argList;
	va_start (argList, nBytesCount);

    int nOldSize = m_nSize;
    EFByte* pb = Realloc (m_nSize + nBytesCount) + nOldSize;
    for (int i = 0; i < nBytesCount; i++)
        *pb++ = (EFByte)va_arg (argList, int);

    va_end (argList);
}

void EF_MiniMem::AddChar (EFChar ch)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFChar))
	{
		*(EFChar*)(m_pData + m_nSize) = ch;
		m_nSize += sizeof (EFChar);
	}
	else
		Append (&ch, sizeof (EFChar));
}

void EF_MiniMem::AddBool (EFBool bl)
{
	if (m_nAllocedSize - m_nSize >= sizeof (EFBool))
	{
		*(EFBool*)(m_pData + m_nSize) = bl;
		m_nSize += sizeof (EFBool);
	}
	else
		Append (&bl, sizeof (EFBool));
}

void EF_MiniMem::AddStr (EFChar* pstr)
{
    MCHECK_MEM_POINTER (pstr)

    if (EF_IsEmptyStr (pstr))
		AddChar (myC ('\0'));
	else
		Append (pstr, (EF_strlen (pstr) + 1) * sizeof (EFChar));
}

void EF_MiniMem::AddText (EFChar* pstr)
{
    MCHECK_MEM_POINTER (pstr)

	if (EF_IsEmptyStr (pstr) == EF_FALSE)
		Append (pstr, EF_strlen (pstr) * sizeof (EFChar));
}

void EF_MiniMem::AddBin (EFBinary pbin)
{
    MCHECK_MEM_POINTER (pbin)

	if (pbin != NULL)
		Append (pbin + sizeof (EFInt) * 3, *(EFInt*)(pbin + sizeof (EFInt) * 2));
}


///////////////////////////////////////////

// if pData == NULL, insert blank data.
EFBool EF_MiniMem::Insert (int nOffset, void* pData, int nSize)
{
    MCHECK_MEM_POINTER (pData)

	if (nSize == 0)  return EF_TRUE;
	if (nOffset == m_nSize)  return Append (pData, nSize);

	int nOldSize = m_nSize;
	EFByte* pMem = Realloc (EF_max (nOffset, nOldSize) + nSize);
	if (pMem == NULL)  return EF_FALSE;

	EFByte* pInsert = pMem + nOffset;
	if (nOffset < nOldSize)
		memmove (pInsert + nSize, pInsert, nOldSize - nOffset);

	if (pData != NULL)
		memmove (pInsert, pData, nSize);
	else
		memset (pInsert, 0, nSize);

	return EF_TRUE;
}

EFBool EF_MiniMem::Append (EF_MiniMem& mem)
{
    MCHECK_MEM_POINTER (mem.GetPtr ())

	return Append (mem.GetPtr (), mem.GetSize ());
}

// if pData == NULL, append blank data.
EFBool EF_MiniMem::Append (void* pData, int nSize)
{
    MCHECK_MEM_POINTER (pData)

	if (nSize == 0)
        return EF_TRUE;

	int nOldSize = m_nSize;
	EFByte* pMem = Realloc (nOldSize + nSize);
	if (pMem == NULL)
        return EF_FALSE;

	EFByte* pAppend = pMem + nOldSize;
	if (pData != NULL)
		memmove (pAppend, pData, nSize);
	else
		memset (pAppend, 0, nSize);

	return EF_TRUE;
}

// if pReplaceData == NULL, replace blank data.
EFBool EF_MiniMem::Replace (int nOffset, int nSize, EFByte* pReplaceData, int nReplaceSize)
{
    MCHECK_MEM_POINTER (pReplaceData)

	int nOldSize = m_nSize;
	if (nSize == 0 || nOffset >= nOldSize)
		return Insert (nOffset, pReplaceData, nReplaceSize);
	if (nReplaceSize == 0)  
		return Remove (nOffset, nSize);

	if (nSize > nOldSize - nOffset)  nSize = nOldSize - nOffset;

	EFByte* pReplaceAt;
	int nMoveSize = nOldSize - nOffset - nSize;
	if (nReplaceSize > nSize)
	{
		pReplaceAt = Realloc (nOldSize + nReplaceSize - nSize);
		if (pReplaceAt == NULL)  return EF_FALSE;
		pReplaceAt += nOffset;

		if (nMoveSize > 0)
			memmove (pReplaceAt + nReplaceSize, pReplaceAt + nSize, nMoveSize);
	}
	else
	{
		pReplaceAt = m_pData;
		pReplaceAt += nOffset;

		if (nReplaceSize < nSize)
		{
			if (nMoveSize > 0)
				memmove (pReplaceAt + nReplaceSize, pReplaceAt + nSize, nMoveSize);

			pReplaceAt = Realloc (nOldSize - (nSize - nReplaceSize));
			if (pReplaceAt == NULL)  return EF_FALSE;  // data already be changed!
			pReplaceAt += nOffset;
		}
	}

	if (pReplaceData != NULL)
		memmove (pReplaceAt, pReplaceData, nReplaceSize);
	else
		memset (pReplaceAt, 0, nReplaceSize);
	return EF_TRUE;
}

// if nOffset == -1, delete data from tail.
EFBool EF_MiniMem::Remove (int nOffset, int nSize)
{
	if (nOffset == -1)
	{
		int nNewSize = EF_max (0, m_nSize - nSize);
		if (nNewSize == 0)
		{
			Free ();
			return EF_TRUE;
		}
		else
			return Realloc (nNewSize) != NULL;
	}

	if (nSize == 0)  return EF_TRUE;
	if (m_pData == NULL || nOffset >= m_nSize)  return EF_FALSE;

	EFByte* pRemoveAt = m_pData + nOffset;

	int nRemoveSize;
	if (nSize < m_nSize - nOffset)
	{
		nRemoveSize = nSize;
		memmove (pRemoveAt, pRemoveAt + nRemoveSize, m_nSize - nOffset - nRemoveSize);
	}
	else
		nRemoveSize = m_nSize - nOffset;

	if (m_nSize == nRemoveSize)
	{
		Free ();
		return EF_TRUE;
	}
	else
		return Realloc (m_nSize - nRemoveSize) != NULL;
}

int EF_MiniMem::ReadFromFile (EFChar* szFileName, int nReadLength)
{
    if (EF_IsEmptyStr (szFileName) == EF_TRUE)
        return -1;

	int nResult = -1;
    EF_MiniMem memBuf;
	FILE* in = fopen (EF_GetMBSStr (szFileName, memBuf), "rb");
	if (in != NULL)
	{
		if (nReadLength == -1)
		{
			fseek (in, 0, SEEK_END);
			nReadLength = (int)ftell (in);
			fseek (in, 0, SEEK_SET);
		}

		if (nReadLength > 0)
		{
			EFByte* p = Alloc (nReadLength);
			if (p != NULL)
			{
				int n = fread (p, 1, nReadLength, in);
				if (!ferror (in))
				{
					if (n != nReadLength)
						Realloc (n);
					fclose (in);
					return n;
				}
			}
		}
		else
			nResult = 0;

		fclose (in);
	}

	Free ();
	return nResult;
}

bool EF_MiniMem::WriteIntoFile (EFChar* szFileName, int nWriteLength)
{
    ASSERT (szFileName != NULL);

    EF_MiniMem memBuf;
	FILE* out = fopen (EF_GetMBSStr (szFileName, memBuf), "wb");
	if (out != NULL)
	{
		int nSize = GetSize ();
		if (nWriteLength == -1)
			nWriteLength = nSize;
		else
			nWriteLength = EF_min (nWriteLength, nSize);

		if (nWriteLength == 0 ||
				(int)fwrite (GetPtr (), 1, nWriteLength, out) == nWriteLength)
		{
			fclose (out);
			return true;
		}

		fclose (out);
	}

	return false;
}

#ifdef _LINUX

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

bool EF_MiniMem::WriteIntoELFFile (EFChar* szFileName, int nWriteLength)
{
    EF_MiniMem memBuf;
	int hfile = open (EF_GetMBSStr (szFileName, memBuf), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
	if (hfile != -1)
    {
	    int nSize = GetSize ();
	    if (nWriteLength == -1)
		    nWriteLength = nSize;
	    else
		    nWriteLength = EF_min (nWriteLength, nSize);

	    if (nWriteLength == 0 ||
			    (int)write (hfile, GetPtr (), nWriteLength) == nWriteLength)
	    {
		    close (hfile);
		    return true;
	    }
	    
	    close (hfile);
    }

	return false;
}

#endif

//////////////////////////////////////////////////
    
#ifdef _DEBUG
    #define MCHECK_STR_POINTER(p)  \
        if ((EFByte*)(p) >= m_mem.GetPtr () && (EFByte*)(p) < m_mem.GetPtr () + m_mem.GetSize ())  \
            FAIL;
#else
    #define MCHECK_STR_POINTER(p)
#endif

EF_CString::EF_CString (const EF_CString& stringSrc)
{
	m_mem.CopyFrom (stringSrc.m_mem.GetPtr (), stringSrc.m_mem.GetSize ());
}

EF_CString::EF_CString (EFChar* ps)
{
	if (EF_IsEmptyStr (ps) == EF_FALSE)
		m_mem.AddStr (ps);
}

EF_CString::EF_CString (EFChar ch)
{
	if (ch != myC('\0'))
	{
		m_mem.AddChar (ch);
		m_mem.AddChar (myC('\0'));
	}
}

void EF_CString::SetDouble (EFDouble db)
{
	m_mem.Empty ();

    EFChar buf [50];
    EF_sprintf (buf, myT ("%.13g"), db);
    m_mem.AddStr (buf);
}

EFDWord EF_CString::GetHash ()
{
    return EF_GetStrHash (GetText ());
}

int EF_CString::GetLength () const
{
    int nSize = m_mem.GetSize ();
	return nSize == 0 ? 0 : nSize / sizeof (EFChar) - 1;
}

EFChar* EF_CString::Detach ()
{
    EFChar* ps = (EFChar*)m_mem.Detach ();
    return ps == NULL ? EF_EMPTY_SZ_TEXT : ps;
}

int EF_CString::GetTextPartDataSize () const
{
    int nSize = m_mem.GetSize ();
    if (nSize > 0)
    {
        ASSERT (nSize >= sizeof (EFChar));
        return nSize - sizeof (EFChar);
    }
    else
        return 0;
}

EFBool EF_CString::IsEmpty () const
{
	return m_mem.GetSize () <= sizeof (EFChar) ? EF_TRUE : EF_FALSE;
}

void EF_CString::Empty()
{
	m_mem.Free ();
}

const EF_CString& EF_CString::operator=(const EF_CString& stringSrc)
{
	m_mem.CopyFrom (stringSrc.m_mem.GetPtr (), stringSrc.m_mem.GetSize ());
	return *this;
}

const EF_CString& EF_CString::operator=(EFChar ch)
{
	m_mem.Empty ();
	if (ch != myC('\0'))
	{
		m_mem.AddChar (ch);
		m_mem.AddChar (myC('\0'));
	}
	return *this;
}

void EF_CString::InsertText (int nIndex, EFChar* ps)
{
    MCHECK_STR_POINTER (ps)

    m_mem.Insert (nIndex * sizeof (EFChar), ps, EF_strlen (ps) * sizeof (EFChar));
}

void EF_CString::InsertText (int nIndex, EFChar* ps, int nLen)
{
    MCHECK_STR_POINTER (ps)

    ASSERT (nLen >= 0 && nLen <= EF_strlen (ps));
	m_mem.Insert (nIndex * sizeof (EFChar), ps, nLen * sizeof (EFChar));
}

void EF_CString::AddText (EFChar* ps)
{
    MCHECK_STR_POINTER (ps)

    if (EF_IsEmptyStr (ps) == EF_FALSE)
    {
        m_mem.Realloc (GetTextPartDataSize ());
	    m_mem.AddStr (ps);
    }
}

void EF_CString::AddText (EFChar* ps, int nLen)
{
    MCHECK_STR_POINTER (ps)
    ASSERT (nLen >= 0);

	if (EF_IsEmptyStr (ps) == EF_FALSE && nLen > 0)
	{
    	m_mem.Realloc (GetTextPartDataSize ());
		m_mem.Append (ps, nLen * sizeof (EFChar));
		m_mem.AddChar (myC('\0'));
	}
}

void EF_CString::SetText (EFChar* ps)
{
    MCHECK_STR_POINTER (ps)

    m_mem.Empty ();
	if (EF_IsEmptyStr (ps) == EF_FALSE)
    	m_mem.AddStr (ps);
}

void EF_CString::SetText (EFChar* ps, int nLen)
{
    MCHECK_STR_POINTER (ps)
    ASSERT (nLen >= 0);

	m_mem.Empty ();
	if (EF_IsEmptyStr (ps) == EF_FALSE && nLen > 0)
	{
		m_mem.Append (ps, nLen * sizeof (EFChar));
		m_mem.AddChar (myC('\0'));
	}
}

#ifdef _UNICODE

void EF_CString::SetText (char* ps, int nLen)
{
    MCHECK_STR_POINTER (ps)

    setlocale (LC_ALL, "");

	m_mem.Empty ();
	if (ps == NULL || *ps == '\0')
		return;

    size_t lenAnsi = nLen == -1 ? strlen (ps) : nLen;

    // 必须这么转换下以确保以0结束，不然mbstowcs会出错。
    EF_MiniMem mem;
    mem.Append (ps, lenAnsi * sizeof (char));
    mem.AddInt (0);
    ps = (char*)mem.GetPtr ();

	int lenWide = (int)mbstowcs (NULL, ps, lenAnsi);
	if (lenWide > 0)
	{
		EFChar* pws = (EFChar*)m_mem.Alloc (sizeof (EFChar) * (lenWide + 1));       
		lenWide = mbstowcs (pws, ps, lenAnsi);
        if (lenWide < 0)
        {
            m_mem.Free ();
        }
        else
        {
    		pws [lenWide] = myC('\0');
            m_mem.Realloc ((lenWide + 1) * sizeof (EFChar));
        }
	}
}

#endif

const EF_CString& EF_CString::operator=(EFChar* ps)
{
    MCHECK_STR_POINTER (ps)

	m_mem.Empty ();
	if (EF_IsEmptyStr (ps) == EF_FALSE)
		m_mem.AddStr (ps);
	return *this;
}

const EF_CString& EF_CString::operator+=(const EF_CString& string)
{
    if (string.IsEmpty () == EF_FALSE)
	{
    	m_mem.Realloc (GetTextPartDataSize ());
		m_mem.Append (string.m_mem.GetPtr (), string.GetTextPartDataSize ());
        if (m_mem.IsEmpty () == EF_FALSE)
    		m_mem.AddChar (myC('\0'));
	}

	return *this;
}

const EF_CString& EF_CString::operator+=(EFChar* ps)
{
    MCHECK_STR_POINTER (ps)

	if (EF_IsEmptyStr (ps) == EF_FALSE)
	{
    	m_mem.Realloc (GetTextPartDataSize ());
		m_mem.Append (ps, (EF_strlen (ps) + 1) * sizeof (EFChar));
	}

	return *this;
}

void EF_CString::RemoveChar (int nIndex, int nLen)
{
    ASSERT (nIndex >= 0 && nLen > 0);
    m_mem.Remove (nIndex * sizeof (EFChar), nLen * sizeof (EFChar));
}

void EF_CString::InsertChar (int nIndex, EFChar ch)
{
    int nLen = GetLength ();
    if (nIndex < 0 || nIndex > nLen)
    {
        FAIL;
        return;
    }

    if (nLen == 0)
    {
        ASSERT (nIndex == 0);
        AddChar (ch);
    }
    else
        m_mem.Insert (nIndex * sizeof (EFChar), (EFByte*)&ch, sizeof (ch));
}

void EF_CString::AddChar (EFChar ch)
{
    m_mem.Realloc (GetTextPartDataSize ());
	m_mem.AddChar (ch);
	m_mem.AddChar (myC ('\0'));
}

const EF_CString& EF_CString::operator+=(EFChar ch)
{
    AddChar (ch);
	return *this;
}

EF_CString EF_CString::Left(int nCount) const
{
	EF_CString str;
	nCount = EF_min (nCount, GetLength ());
	if (nCount > 0)
	{
		str.m_mem.Append (m_mem.GetPtr (), nCount * sizeof (EFChar));
		str.m_mem.AddChar (myC('\0'));
	}

	return str;
}

EF_CString EF_CString::Right(int nCount) const
{
	EF_CString str;
	nCount = EF_min (nCount, GetLength ());
	if (nCount > 0)
		str.m_mem.Append (m_mem.GetPtr () + (GetLength () - nCount) * sizeof (EFChar), (nCount + 1) * sizeof (EFChar));

	return str;
}

void EF_CString::MakeUpper()
{
	EFChar* ps = (EFChar*)m_mem.GetPtr ();
	int nLen = GetLength ();
	for (int i = 0; i < nLen; ps++, i++)
	{
		if (*ps >= myC ('a') && *ps <= myC ('z'))
			*ps = *ps - myC ('a') + myC ('A');
	}
}

void EF_CString::MakeLower()
{
	EFChar* ps = (EFChar*)m_mem.GetPtr ();
	int nLen = GetLength ();
	for (int i = 0; i < nLen; ps++, i++)
	{
		if (*ps >= myC ('A') && *ps <= myC ('Z'))
			*ps = *ps - myC ('A') + myC ('a');
	}
}

void EF_CString::TrimRight()
{
	EFChar* ps = (EFChar*)m_mem.GetPtr ();
	int nLen = GetLength (), i;
	for (i = nLen - 1; i >= 0; i--)
	{
		if (ps [i] != myC (' '))
			break;
	}

	if (i + 1 != nLen)
	{
		m_mem.Realloc ((i + 1) * sizeof (EFChar));
		m_mem.AddChar (myC ('\0'));
	}
}

void EF_CString::TrimLeft()
{
	EFChar* ps = (EFChar*)m_mem.GetPtr ();
	int nLen = GetLength (), i;
	for (i = 0; i < nLen; i++)
	{
		if (ps [i] != myC(' '))
			break;
	}

	if (i > 0)
	{
		nLen = (nLen - i + 1) * sizeof (EFChar);
		memmove (ps, ps + i, nLen);
		m_mem.Realloc (nLen);
	}
}

EFBool EF_CString::EndOf (EFChar* ps)
{
	int nLen = GetLength ();
	int nStrLen = EF_strlen (ps);
	return (nStrLen == 0 || nLen >= nStrLen &&
		EF_strcmp ((EFChar*)(m_mem.GetPtr () + (nLen - nStrLen) * sizeof (EFChar)), ps) == 0) ? EF_TRUE : EF_FALSE;
}

EFBool EF_CString::IEndOf (EFChar* ps)
{
	int nLen = GetLength ();
	int nStrLen = EF_strlen (ps);
	return (nStrLen == 0 || nLen >= nStrLen &&
		EF_stricmp ((EFChar*)(m_mem.GetPtr () + (nLen - nStrLen) * sizeof (EFChar)), ps) == 0) ? EF_TRUE : EF_FALSE;
}

int EF_CString::myicmp (EFChar* ps)
{
	return EF_stricmp ((EFChar*)m_mem.GetPtr (), ps);
}

int EF_CString::mycmp (EFChar* ps)
{
	return EF_strcmp ((EFChar*)m_mem.GetPtr (), ps);
}

void EF_CString::FormatV (EFChar* szFormat, va_list argList)
{
    MCHECK_STR_POINTER (szFormat)

    EF_MiniMem memTemp;
	va_list argListSave = argList;

	// 获取最多可能需要分配的内存尺寸
	int nMaxLen = 0;
    EFChar* ps;
	for (ps = szFormat; *ps != myC('\0'); ps++)
	{
		// 管理'%'字符
		if (*ps != myC('%'))
		{
			nMaxLen++;
			continue;
		}

        ps++;

		// 跳过'%%'
        if (*ps == myC('%'))
        {
			nMaxLen++;
			continue;
        }

        ////////////////////////////////////////////////////

		int nItemLen = 0;

		// 管理格式指定
		int nWidth = 0;
		for (; *ps != myC('\0'); ps++)
		{
			// 检查是否有效的标记
			if (*ps == myC('#'))
				nMaxLen += 2;   // '0x'
			else if (*ps == myC('*'))
				nWidth = va_arg (argList, int);
			else if (*ps == myC('-') || *ps == myC('+') || *ps == myC('0') || *ps == myC(' '))
				;
			else
				break;
		}

        // 获取宽度并跳过该指定符
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = EF_atoi (ps);
			while (*ps != '\0' && *ps >= myC('0') && *ps <= myC('9'))
                ps++;
		}
		ASSERT (nWidth >= 0);

		int nPrecision = 0;
		if (*ps == myC('.'))
		{
			// 跳过'.'指定符(width.precision)
			ps++;

			// 获取精度并跳过该指定符
			if (*ps == myC('*'))
			{
				nPrecision = va_arg (argList, int);
				ps++;
			}
			else
			{
				nPrecision = EF_atoi (ps);
			    while (*ps != '\0' && *ps >= myC('0') && *ps <= myC('9'))
                    ps++;
			}
			ASSERT (nPrecision >= 0);
		}

		// 处理类型指定符
		switch (*ps)
		{
		// 忽略字符类型指定，强制都为unicode字符
		case myC('h'):
		case myC('l'):
		case myC('F'):
		case myC('N'):
		case myC('L'):
			ps++;
			break;
		}

		switch (*ps)
		{
		case myC('c'):
		case myC('C'):
			nItemLen = 2;
			// va_arg (argList, EFChar);  // 此行gcc会报警告，用下行替换。
			va_arg (argList, int);
			break;

    #ifndef _UNICODE
        case myC('s'):  {
			char* pstrNextArg = va_arg (argList, char*);
            MCHECK_STR_POINTER (pstrNextArg)
			if (pstrNextArg == NULL)
			   nItemLen = 6;  // "(null)"
			else
			   nItemLen = EF_max (1, strlen (pstrNextArg));
            break;  }

        case myC('S'):  {
			EFChar* pstrNextArg = va_arg (argList, EFChar*);
            MCHECK_STR_POINTER (pstrNextArg)
			if (pstrNextArg == NULL)
			   nItemLen = 6;  // "(null)"
			else
			   nItemLen = EF_max (1, EF_strlen (pstrNextArg));
            break;  }
		}
    #else
        case myC('s'):  {
			EFChar* pstrNextArg = va_arg (argList, EFChar*);
            MCHECK_STR_POINTER (pstrNextArg)
			if (pstrNextArg == NULL)
			   nItemLen = 6;  // "(null)"
			else
			   nItemLen = EF_max (1, EF_strlen (pstrNextArg));
            break;  }

        case myC('S'):  {
			char* pstrNextArg = va_arg (argList, char*);
            MCHECK_STR_POINTER (pstrNextArg)
			if (pstrNextArg == NULL)
			   nItemLen = 6;  // "(null)"
			else
			   nItemLen = EF_max (1, strlen (pstrNextArg));
            break;  }
		}
    #endif

		// 调整文本长度
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = EF_min (nItemLen, nPrecision);
			nItemLen = EF_max (nItemLen, nWidth);
		}
		else
		{
			switch (*ps)
			{
			// 整数
			case myC('d'):
			case myC('i'):
			case myC('u'):
			case myC('x'):
			case myC('X'):
			case myC('o'):
				va_arg (argList, int);
				nItemLen = 32;
				nItemLen = EF_max (nItemLen, nWidth + nPrecision);
				break;

			case myC('e'):
			case myC('g'):
			case myC('G'):
				va_arg (argList, double);
				nItemLen = 128;
				nItemLen = EF_max (nItemLen, nWidth + nPrecision);
				break;

            case myC('f'):  {
				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == max precision of a double
				// 6 == adjustment in case precision is not specified,
				//   which means that the precision defaults to 6
                EFChar* pszTemp = (EFChar*)memTemp.Alloc (EF_max (nWidth, 312 + nPrecision + 6));
				double f = va_arg (argList, double);
				EF_sprintf (pszTemp, myT ("%*.*f"), nWidth, nPrecision + 6, f);
				nItemLen = EF_strlen (pszTemp);
                break;  }

			case myC('p'):
				va_arg (argList, void*);
				nItemLen = 32;
				nItemLen = EF_max (nItemLen, nWidth + nPrecision);
				break;

			// 不输出
			case myC('n'):
				va_arg (argList, int*);
				break;

			DEFAULT_FAIL  // 未知格式选项
			}
		}

		// 调整nMaxLen
        nMaxLen += nItemLen;
	}

    ////////////////////////////////////////////////////

	ps = (EFChar*)m_mem.Alloc ((nMaxLen + 1) * sizeof (EFChar));

    int n = EF_vsprintf (ps, szFormat, argListSave);
	ASSERT (n <= nMaxLen);
    // 删除过多的空余内存
    m_mem.Realloc ((n + 1) * sizeof (EFChar));

	va_end (argListSave);
}

void EF_CString::Format (EFChar* szFormat, ...)
{
	va_list argList;
	va_start (argList, szFormat);
	FormatV (szFormat, argList);
	va_end (argList);
}

EF_CString operator+(const EF_CString& string1, const EF_CString& string2)
{
	EF_CString str;
	str.m_mem.Append (string1.m_mem.GetPtr (), string1.GetTextPartDataSize ());
	str.m_mem.Append (string2.m_mem.GetPtr (), string2.GetTextPartDataSize ());
	if (str.m_mem.GetSize () > 0)
		str.m_mem.AddChar (myC ('\0'));
	return str;
}

EF_CString operator+(const EF_CString& string, EFChar ch)
{
	EF_CString str;
	str.m_mem.Append (string.m_mem.GetPtr (), string.GetTextPartDataSize ());
	if (ch != myC ('\0'))
		str.m_mem.AddChar (ch);
	if (str.m_mem.GetSize () > 0)
		str.m_mem.AddChar (myC ('\0'));
	return str;
}

EF_CString operator+(const EF_CString& string, EFChar* ps)
{
	EF_CString str;
	str.m_mem.Append (string.m_mem.GetPtr (), string.GetTextPartDataSize ());
	if (ps != NULL)
		str.m_mem.Append (ps, EF_strlen (ps) * sizeof (EFChar));
	if (str.m_mem.GetSize () > 0)
		str.m_mem.AddChar (myC ('\0'));
	return str;
}

EF_CString operator+(EFChar* ps, const EF_CString& string)
{
	EF_CString str;
	if (ps != NULL)
		str.m_mem.Append (ps, EF_strlen (ps) * sizeof (EFChar));
	str.m_mem.Append (string.m_mem.GetPtr (), string.GetTextPartDataSize ());
	if (str.m_mem.GetSize () > 0)
		str.m_mem.AddChar (myC ('\0'));
	return str;
}

EF_CString operator+(EFChar ch, const EF_CString& string)
{
	EF_CString str;
    str.m_mem.AddChar (ch);
	str.m_mem.Append (string.m_mem.GetPtr (), string.GetTextPartDataSize ());
	str.m_mem.AddChar (myC ('\0'));
	return str;
}

EFBool operator==(const EF_CString& s1, const EF_CString& s2)
{
	return EF_strcmp ((EFChar*)s1.m_mem.GetPtr (), (EFChar*)s2.m_mem.GetPtr ()) == 0 ? EF_TRUE : EF_FALSE;
}

EFBool operator==(const EF_CString& s1, EFChar* s2)
{
	return EF_strcmp ((EFChar*)s1.m_mem.GetPtr (), s2) == 0 ? EF_TRUE : EF_FALSE;
}

EFBool operator==(EFChar* s1, const EF_CString& s2)
{
	return EF_strcmp ((EFChar*)s2.m_mem.GetPtr (), s1) == 0 ? EF_TRUE : EF_FALSE;
}

EFBool operator!=(const EF_CString& s1, const EF_CString& s2)
{
	return EF_strcmp ((EFChar*)s1.m_mem.GetPtr (), (EFChar*)s2.m_mem.GetPtr ()) != 0 ? EF_TRUE : EF_FALSE;
}

EFBool operator!=(const EF_CString& s1, EFChar* s2)
{
	return EF_strcmp ((EFChar*)s1.m_mem.GetPtr (), s2) != 0 ? EF_TRUE : EF_FALSE;
}

EFBool operator!=(EFChar* s1, const EF_CString& s2)
{
	return EF_strcmp ((EFChar*)s2.m_mem.GetPtr (), s1) != 0 ? EF_TRUE : EF_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void EF_CStringArray::Append (EF_CStringArray& src)
{
    MCHECK_MEM_POINTER (src.GetPtr ())

	EFChar** ppsAppend = src.GetData ();
	int nAppendCount = src.GetCount ();

	int nOldCount = GetCount ();
	EF_MiniMem::Append (NULL, nAppendCount * sizeof (EFChar*));

	EFChar** pps = &((EFChar**)m_pData) [nOldCount];
	for (int i = 0; i < nAppendCount; i++)
	{
		pps [i] = EF_CloneStr (ppsAppend [i]);
	}
}

void EF_CStringArray::Copy (EF_CStringArray& src)
{
    MCHECK_MEM_POINTER (src.GetPtr ())

	RemoveAll ();
	Append (src);
}

void EF_CStringArray::RemoveAll ()
{
	EFChar** pps = GetData ();
	int nCount = GetCount ();
	for (int i = 0; i < nCount; i++)
		EF_free (pps [i]);

	EF_MiniMem::Free ();
}

void EF_CStringArray::RemoveAt (int nIndex, int nCount)
{
	ASSERT (nIndex >= 0 && nIndex + nCount <= GetCount ());

	EFChar** pps = &((EFChar**)m_pData) [nIndex];
	for (int i = 0; i < nCount; i++)
		EF_free (pps [i]);

	EF_MiniMem::Remove (nIndex * sizeof (EFChar*), sizeof (EFChar*) * nCount);
}

void EF_CStringArray::RemoveAt_NotDelete (int nIndex, int nCount)
{
	ASSERT (nIndex >= 0 && nIndex + nCount <= GetCount ());
	EF_MiniMem::Remove (nIndex * sizeof (EFChar*), sizeof (EFChar*) * nCount);
}

void EF_CStringArray::SetAt (int nIndex, EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	SetAt_NoClone (nIndex, EF_CloneStr (szElement));
}

void EF_CStringArray::Add (EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	Add_NoClone (EF_CloneStr (szElement));
}

void EF_CStringArray::Add (EFChar* szElement, int nLen)
{
    MCHECK_MEM_POINTER (szElement)

	Add_NoClone (EF_CloneStr (szElement, nLen));
}

void EF_CStringArray::InsertAt (int nIndex, EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	InsertAt_NoClone (nIndex, EF_CloneStr (szElement));
}

void EF_CStringArray::InsertAt (int nIndex, EFChar* szElement, int nLen)
{
    MCHECK_MEM_POINTER (szElement)

	InsertAt_NoClone (nIndex, EF_CloneStr (szElement, nLen));
}

#ifdef _UNICODE

void EF_CStringArray::SetAt (int nIndex, char* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	SetAt_NoClone (nIndex, EF_CloneStr (szElement));
}

void EF_CStringArray::Add (char* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	Add_NoClone (EF_CloneStr (szElement));
}

void EF_CStringArray::InsertAt (int nIndex, char* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	InsertAt_NoClone (nIndex, EF_CloneStr (szElement));
}

#endif

void EF_CStringArray::SetAt_NoClone (int nIndex, EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	if (nIndex >= 0 && nIndex < GetCount ())
	{
		EFChar** pps = &((EFChar**)m_pData) [nIndex];
		EF_free (*pps);
		*pps = szElement;
	}
}

void EF_CStringArray::Add_NoClone (EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	EF_MiniMem::Append (&szElement, sizeof (EFChar*));
}

void EF_CStringArray::InsertAt_NoClone (int nIndex, EFChar* szElement)
{
    MCHECK_MEM_POINTER (szElement)

	EF_MiniMem::Insert (nIndex * sizeof (EFChar*), (EFByte*)&szElement, sizeof (EFChar*));
}

void EF_CStringArray::Append_NoClone (EF_CStringArray& src)
{
    MCHECK_MEM_POINTER (src.GetPtr ())

	EF_MiniMem::Append (src.GetPtr (), src.GetSize ());
	src.EF_MiniMem::Free ();
}

void EF_CStringArray::Append_NoClone (EF_CStringArray* psrc)
{
    MCHECK_MEM_POINTER (psrc->GetPtr ())

	EF_MiniMem::Append (psrc->GetPtr (), psrc->GetSize ());
	psrc->EF_MiniMem::Free ();
}

void EF_CStringArray::Copy_NoClone (EF_CStringArray& src)
{
    MCHECK_MEM_POINTER (src.GetPtr ())

	RemoveAll ();
	Append_NoClone (src);
}

int EF_CStringArray::Find (EFChar* ps)
{
    if (ps != NULL)
    {
	    int nCount = GetCount ();
	    for (int i = 0; i < nCount; i++)
	    {
		    if (EF_strcmp (GetAt (i), ps) == 0)
			    return i;
	    }
    }

	return -1;
}

