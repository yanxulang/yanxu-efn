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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#ifndef _LINUX
    #include <windows.h>
#endif

#include <ef/ef.h>
#include <ef/functions.h>
#include <eflib/sys/runtime.h>

EF_DEFINE_EMPTY_CONST_DATA

int EF_atoi (EFChar* ps)
{
    if (ps == NULL)
        return 0;

    while (*ps == myC(' '))
        ps++;

    EFDWord c = (EFDWord)*ps++;
    EFDWord sign = c;
    if (c == myC('-') || c == myC('+'))
        c = (EFDWord)*ps++;

    long total = 0;
    while (c >= (EFDWord)myC('0') && c <= (EFDWord)myC('9'))
    {
        total = 10 * total + (c - (EFDWord)myC('0'));
        c = (EFDWord)*ps++;
    }

    if (sign == myC('-'))
        return -total;
    else
        return total;
}

#ifdef _UNICODE

double EF_atof (EFChar* ps)
{
    EF_MiniMem memBuf;
    return atof (EF_GetMBSStr (ps, memBuf));
}

#else

double EF_atof (EFChar* ps)
{
    return atof (ps);
}

#endif

int EF_strlen (EFChar* ps)
{
    if (ps == NULL)
        return 0;

    EFChar* ps2 = ps;
    while (*ps2++);

    return ps2 - ps - 1;
}

int EF_strlen (EFText etext)
{
    if (etext == NULL)
        return 0;
    else
        return ((EFInt*)etext) [1];
}

void EF_strcpy (EFChar* szDest, EFChar* szSource)
{
    ASSERT (szDest != NULL && szSource != szDest);

	if (szSource == NULL)
        *szDest = myC('\0');
    else
        while (*szDest++ = *szSource++);
}

void EF_strncpy (EFChar* szDest, EFChar* szSource, int nCount)
{
    ASSERT (szDest != NULL && szSource != szDest && nCount >= 0);

    if (szSource == NULL)
        szSource = myT ("");
#ifdef _UNICODE
	wcsncpy (szDest, szSource, nCount);
#else
	strncpy (szDest, szSource, nCount);
#endif
}

void EF_strcat (EFChar* szDest, EFChar* szSource)
{
    ASSERT (szDest != NULL && szSource != szDest);

    if (szSource == NULL)
        szSource = myT ("");
#ifdef _UNICODE
	wcscat (szDest, szSource);
#else
	strcat (szDest, szSource);
#endif
}

void EF_strncat (EFChar* szDest, EFChar* szSource, int nCount)
{
    ASSERT (szDest != NULL && szSource != szDest && nCount >= 0);

    if (szSource == NULL)
        szSource = myT ("");
#ifdef _UNICODE
	wcsncat (szDest, szSource, nCount);
#else
	strncat (szDest, szSource, nCount);
#endif
}

int EF_stricmp (EFChar* szSource, EFChar* szDest)
{
	if (szSource == NULL)  szSource = myT ("");
	if (szDest == NULL)  szDest = myT ("");

	EFChar ch1, ch2;
	do
	{
		ch2 = ((*szSource <= myC ('Z')) && (*szSource >= myC ('A'))) ? *szSource + myC ('a') - myC ('A') : *szSource;
		ch1 = ((*szDest <= myC ('Z')) && (*szDest >= myC ('A'))) ? *szDest + myC ('a') - myC ('A') : *szDest;
		szSource++;
		szDest++;
	} while (ch2 && ch2 == ch1);

	return (int)(ch2 - ch1);
}

int EF_strcmp (EFChar* szSource, EFChar* szDest)
{
	if (szSource == NULL)  szSource = myT ("");
	if (szDest == NULL)  szDest = myT ("");

	int nResult = 0;
	while (!(nResult = (int)(*szSource - *szDest)) && *szDest)
    {
		szSource++;
        szDest++;
    }

	return nResult;
}

int EF_strncmp (EFChar* ps1, EFChar* ps2, int count)
{
    if (!count)
        return 0;

    while (--count && *ps1 && *ps1 == *ps2)
    {
        ps1++;
        ps2++;
    }

    return (int)(*ps1 - *ps2);
}

int EF_strnicmp (EFChar* ps1, EFChar* ps2, int count)
{
	if (!count)
		return 0;

	while (--count && *ps1 && EF_tolowerc (*ps1) == EF_tolowerc (*ps2))
	{
		ps1++;
		ps2++;
	}

	return (int)(EF_tolowerc (*ps1) - EF_tolowerc (*ps2));
}

EFChar* EF_toupper (EFChar* pText)
{
    if (pText != NULL)
    {
        EFChar* ps = pText;
	    while (true)
	    {
		    EFChar c = *ps;
            if (c == myC('\0'))
                break;
            if ((EFDWord)c >= (EFDWord)myC('a') && (EFDWord)c <= (EFDWord)myC('z'))
                *ps = c - myC('a') + myC('A');
            ps++;
	    }
    }

	return pText;
}

EFChar* EF_tolower(EFChar* pText)
{
    if (pText != NULL)
    {
        EFChar* ps = pText;
	    while (true)
	    {
		    EFChar c = *ps;
            if (c == myC('\0'))
                break;
            if ((EFDWord)c >= (EFDWord)myC('A') && (EFDWord)c <= (EFDWord)myC('Z'))
                *ps = c - myC('A') + myC('a');
            ps++;
	    }
    }

	return pText;
}

EFChar EF_toupperc(EFChar c)
{
	if(c >= myC('a') && c <= myC('z'))
		return (EFChar)(c - myC('a') + myC('A'));
	else
		return c;
}

EFChar EF_tolowerc(EFChar c)
{
	if(c >= myC('A') && c <= myC('Z'))
		return (EFChar) (c - myC('A') + myC('a'));
	else
		return c;
}

EFChar* EF_strchr (EFChar* ps, EFChar ch)
{
	if (ps == NULL)
		return NULL;

	while (*ps && *ps != ch)
		ps++;

	if (*ps == ch)
		return ps;
	else
		return NULL;
}

EFChar* EF_strrchr (EFChar* ps, EFChar ch)
{
	if (ps == NULL)
		return NULL;

	EFChar* start = ps;

	while (*ps++);
	while (--ps != start && *ps != ch);

	if (*ps == ch)
		return ps;
	else
		return NULL;
}

EFChar* EF_strstr (EFChar* ps1, EFChar* ps2)
{
	if (ps1 == NULL)  ps1 = myT("");
	if (ps2 == NULL)  ps2 = myT("");

	EFChar* cp = ps1;
	EFChar *s1, *s2;

	while (*cp)
	{
		s1 = cp;
		s2 = ps2;

		while (*s1 && *s2 && !(*s1 - *s2))
        {
			s1++;
            s2++;
        }

		if (!*s2)
			return cp;

		cp++;
	}

	return NULL;
}

// 注意EF_strtok将修改ps所指向的内容。
EFChar* EF_strtok (EFChar * ps, EFChar* szControl)
{
	EFChar *szToken;
	EFChar *szCtl;

	static EFChar *s_nk;

	if (!ps)
		ps = s_nk;

	while (*ps)
	{
		for (szCtl=szControl; *szCtl && *szCtl != *ps; szCtl++);
		if (!*szCtl)  break;
		ps++;
	}

	szToken = ps;
	while (*ps)
	{
		for (szCtl=szControl; *szCtl && *szCtl != *ps; szCtl++);
		if (*szCtl)
		{
			*ps++ = '\0';
			break;
		}
        ps++;
	}

	s_nk = ps;
	if (szToken == ps)
		return NULL;
	else
		return szToken;
}

EFBool EF_IsEmptyStr (EFChar* ps)
{
	return (ps == NULL || *ps == myC ('\0')) ? EF_TRUE : EF_FALSE;
}

EFBool EF_IsEmptyText (EFText etext)
{
	return (EF_strlen (etext) == 0) ? EF_TRUE : EF_FALSE;
}

EFBool EF_IsEmptyBin (EFBinary bin)
{
	return (bin == NULL || ((EFInt*)bin) [2] == 0) ? EF_TRUE : EF_FALSE;
}

EFBool EF_IsEmptyArray (EFArray array)
{
	return (array == NULL || ((EFInt*)array) [1] == 1 && ((EFInt*)array) [2] == 0) ? EF_TRUE : EF_FALSE;
}

int EF_max (int n1, int n2)
{
	return n1 > n2 ? n1 : n2;
}

int EF_min (int n1, int n2)
{
	return n1 < n2 ? n1 : n2;
}

#ifdef _UNICODE

#include <locale.h>

char* EF_GetMBSStr (EFChar* ps, EF_MiniMem& memBuf)
{
	if (EF_IsEmptyStr (ps))
		return EF_EMPTY_SZ_ANSI_TEXT;

    setlocale (LC_ALL, "");

    int nSize = (EF_strlen (ps) + 1) * sizeof (char) * 4;
	char* buf = (char*)memBuf.Alloc (nSize);
    memset (buf, 0, nSize);
	wcstombs (buf, ps, nSize);
	return buf;
}

#else

char* EF_GetMBSStr (EFChar* ps, EF_MiniMem& memBuf)
{
	return ps;
}

#endif

// E str -> E str
EFText EF_CloneEFText (EFText etext)
{
	if (etext == NULL)
		return EF_EMPTY_TEXT;
    else
        return EF_CloneEFText (EF_GET_TEXT (etext));
}

// C str -> E str;
EFText EF_CloneEFText (EFChar* ps)
{
	return EF_CloneEFText (ps, EF_strlen (ps));
}

EFText EF_CloneEFText (EFChar* ps, int nLen)
{
	if (EF_IsEmptyStr (ps) || nLen <= 0)
		return EF_EMPTY_TEXT;

	ASSERT(nLen <= EF_strlen (ps));

	int nSize = nLen * sizeof (EFChar);
	EFInt* etext = (EFInt*)EF_malloc (sizeof (EFInt) + sizeof (EFInt) + nSize + sizeof (EFChar));
    etext[0] = (EF_GCDT_PUREDATA | 0);
	etext[1] = nLen;
	memcpy (&etext[2], ps, nSize);
    *(EFChar*)((EFByte*)etext + sizeof (EFInt) + sizeof (EFInt) + nSize) = myC('\0');

	return (EFText)etext;
}

EFChar* EF_CloneEFTextToStr (EFText etext)
{
    if (etext == NULL)
        return EF_EMPTY_SZ_TEXT;
    EFChar* ps = EF_GET_TEXT(etext);
    return EF_CloneStr (ps, EF_strlen (ps));
}

//C str -> C str
EFChar* EF_CloneStr (EFChar* text)
{
	return EF_CloneStr (text, EF_strlen (text));
}

EFChar* EF_CloneStr (EFChar* text, EFInt len)
{
	if (text == NULL || len <= 0)
        return EF_EMPTY_SZ_TEXT;

	ASSERT(len <= EF_strlen (text));

	EFChar* szNew = (EFChar*) EF_malloc ((len + 1) * sizeof (EFChar));
	memcpy (szNew, text, len * sizeof (EFChar));
	szNew [len] = myC('\0');
	return szNew;
}

#ifdef _UNICODE

EFChar* EF_CloneStr (char* ps)
{
	if (ps == NULL || *ps == '\0')
		return EF_EMPTY_SZ_TEXT;

    setlocale (LC_ALL, "");

	int lenAnsi = strlen (ps);
	int lenWide = (int)mbstowcs (NULL, ps, lenAnsi);
	if (lenWide <= 0)
		return EF_EMPTY_SZ_TEXT;

	EFChar* pws = (EFChar*)EF_malloc (sizeof (EFChar) * (lenWide + 1));
	int lenWideNew = mbstowcs (pws, ps, lenAnsi);
    if (lenWideNew <= 0)
    {
        EF_free (pws);
		return EF_EMPTY_SZ_TEXT;
    }
	pws [lenWideNew] = myC('\0');
    if (lenWideNew != lenWide)
        pws = (EFChar*)EF_realloc (pws, sizeof (EFChar) * (lenWideNew + 1));

	return pws;
}

EFText EF_CloneEFText (char* ps)
{
	if (ps == NULL || *ps == '\0')
		return EF_EMPTY_TEXT;

    setlocale (LC_ALL, "");

    int lenAnsi = strlen (ps);
	int lenWide = (int)mbstowcs (NULL, ps, lenAnsi);
	if (lenWide <= 0)
		return EF_EMPTY_TEXT;

    EFInt* etext = (EFInt*)EF_malloc (sizeof (EFInt) + sizeof (EFInt) + sizeof (EFChar) * (lenWide + 1));
    etext[0] = (EF_GCDT_PUREDATA | 0);
	etext[1] = lenWide;
	EFChar* pws = EF_GET_TEXT (etext);
	int lenWideNew = mbstowcs (pws, ps, lenAnsi);
	if (lenWideNew <= 0)
    {
        EF_free (etext);
		return EF_EMPTY_TEXT;
    }
	pws [lenWideNew] = myC('\0');
    if (lenWideNew != lenWide)
        etext = (EFInt*)EF_realloc (etext, sizeof (EFInt) + sizeof (EFInt) + sizeof (EFChar) * (lenWideNew + 1));

	return (EFText)etext;
}

#endif

int EF_GetEFBinLength (EFBinary bin)
{
    return bin == NULL ? 0 : ((EFInt*)bin) [2];
}

EFByte* EF_GetEFBinData (EFBinary bin)
{
    return bin == NULL ? NULL : ((EFByte*)bin + sizeof (EFInt) * 3);
}

EFBinary EF_CloneEFBin (EFBinary bin)
{
	return EF_CloneEFBin (EF_GetEFBinData (bin), EF_GetEFBinLength (bin));
}

// 如果pData为NULL，则分配返回指定尺寸内容为字节0的EFBinary。
EFBinary EF_CloneEFBin (void* pData, EFInt nDataLength)
{
	if (nDataLength <= 0)
		return EF_EMPTY_BIN;

	EFBinary pBinData = (EFBinary)EF_malloc (nDataLength + sizeof (EFInt) * 3);
	ASSERT (pBinData);

	*(EFInt*)pBinData = (EF_GCDT_PUREDATA | 0);
	*(EFInt*)(pBinData + sizeof (EFInt)) = 1;
	*(EFInt*)(pBinData + sizeof (EFInt) * 2) = nDataLength;
    if (pData == NULL)
        memset (pBinData + sizeof (EFInt) * 3, 0, nDataLength);
    else
    	memcpy (pBinData + sizeof (EFInt) * 3, pData, nDataLength);

	return pBinData;
}

///////////////////////////////////////////////////////////////////////////////

int EF_GetDatatypeSize (EFChar* szDataType, EFDWord dwDataTypeHash)
{
    if (EF_IsEmptyStr (szDataType) == EF_TRUE)
    {
        FAIL;
        return 0;
    }

    static EFDWord s_dwDoubleDataTypeHashValue = EF_GetStrHash (EF_DOUBLE);

    ASSERT (dwDataTypeHash != 0);
	if (dwDataTypeHash == s_dwDoubleDataTypeHashValue && EF_strcmp (szDataType, EF_DOUBLE) == 0)
	{
        ASSERT (sizeof (EFDouble) == 8);
		return sizeof (EFDouble);
	}
	else
	{
		//   备注，为了提高效率，此处没有细分，在32位系统下没有问题，
		// 在其它系统下改进下对其细分或者保证其它数据类型和EFInt尺寸一样即可。
        ASSERT (sizeof (EFInt) == 4);
		return sizeof (EFInt);
	}
}

int EF_GetDatatypeSize (EF_DatatypeType dtType)
{
    ASSERT (sizeof (EFDouble) == 8 && sizeof (EFInt) == 4);

    if (dtType == EF_DT_DOUBLE)
	{
		return sizeof (EFDouble);
	}
	else
	{
		//   备注，为了提高效率，此处没有细分，在32位系统下没有问题，
		// 在其它系统下改进下对其细分或者保证其它数据类型和EFInt尺寸一样即可。
		return sizeof (EFInt);
	}
}

int EF_GetDatatypeSizeOfArrayElement (EF_DatatypeType dtType)
{
    ASSERT (sizeof (EFDouble) == 8 && sizeof (EFInt) == 4);

	if (dtType == EF_DT_DOUBLE || dtType == EF_DT_DOUBLE_ARRAY)
	{
		return sizeof (EFDouble);
	}
	else
	{
		//   备注，为了提高效率，此处没有细分，在32位系统下没有问题，
		// 在其它系统下改进下对其细分或者保证其它数据类型和EFInt尺寸一样即可。
		return sizeof (EFInt);
	}
}

///////////////////////////////////////////////////////////////////////////////

void* EF_GetAddressByIntIndex (void* pData, int index)
{
    ASSERT (pData != NULL && index >= 0);
	return (void*)((EFByte*)pData + index * sizeof (EFInt));
}

EFInt EF_GetIntByIndex (void* pData, int index)
{
    ASSERT (pData != NULL && index >= 0);
	return *(EFInt*)((EFByte*)pData + index * sizeof (EFInt));
}

void EF_SetIntByIndex (void* pData, int index, EFInt value)
{
    ASSERT (pData != NULL && index >= 0);
	*(EFInt*)EF_GetAddressByIntIndex (pData, index) = value;
}

void* EF_GetEFBinDataAndLength (EFBinary pBin, EFInt* pLength)
{
    if (pBin == NULL)
        pBin = EF_EMPTY_BIN;

	if (pLength != NULL)
        *pLength = 0;

	ASSERT (EF_GetIntByIndex (pBin, 1) == 1);
	EFInt n = EF_GetIntByIndex (pBin, 2);
	if (pLength != NULL)
        *pLength = n;
	return EF_GetAddressByIntIndex (pBin, 3);
}

EFChar* EF_GetPureDatatype (EFChar* szDataType, EFBool* pblIsAry)
{
	if (pblIsAry != NULL)
		*pblIsAry = EF_FALSE;

	if (EF_IsEmptyStr (szDataType))
		return szDataType;

	if (*szDataType == EF_ARRAY_DATA_TYPE_LEADER_CHAR)
	{
		if (pblIsAry != NULL)
			*pblIsAry = EF_TRUE;
		szDataType++;
	}

	return szDataType;
}

int EF_GetEFArraySize (EFArray array, void** ppAryData)
{
    EFInt* pnAryInf = (EFInt*)array;
	if (pnAryInf == NULL)
    {
	    if (ppAryData != NULL)
		    *ppAryData = NULL;
		return 0;
    }

    pnAryInf++;

	EFInt nAryDim = *pnAryInf++;
    ASSERT (nAryDim > 0);

    int n;
	if (nAryDim <= 0)
    {
		n = 0;
    }
    else
    {
        n = *pnAryInf++;
        nAryDim--;

	    while (nAryDim > 0)
	    {
		    n *= *pnAryInf++;
		    nAryDim--;
	    }
    }

	if (ppAryData != NULL)
		*ppAryData = pnAryInf;

	return n;
}

void* EF_GetEFArrayData (EFArray array, int* pnArraySize/*=NULL*/)
{
	void* pData = NULL;
	int size = EF_GetEFArraySize (array, (void**) &pData);
	if (pnArraySize)
		*pnArraySize = size;
	return pData;
}

int EF_GetEFArrayDim(EFArray array)
{
	return ((int*)array) [1];
}

extern EFClassInfo* ef_pRuntimeClassInfo;

// szClsLibName提供出错代码所在类库的名称。
void EF_ReportEFRuntimeError (EFChar* szClsLibName, EFInt nErrorCode, EFChar* szErrorText)
{
    if (ef_pRuntimeClassInfo != NULL)
    {
        sys_Runtime_ReportError2 (szClsLibName, nErrorCode, szErrorText);
    }
    else if (EF_IsEmptyStr (szClsLibName) == EF_TRUE)
    {
        if (EF_IsEmptyStr (szErrorText) == EF_TRUE)
        {
	        if (nErrorCode < 0)
	        {
		        EF_fprintf (stderr, myT ("发生错误码为%d的严重错误\n"), nErrorCode);
		        exit (-1);
	        }
            else
		        EF_fprintf (stderr, myT ("发生错误码为%d的错误\n"), nErrorCode);
        }
        else
        {
	        if (nErrorCode < 0)
	        {
		        EF_fprintf (stderr, myT ("严重错误(%d): %s\n"), nErrorCode, szErrorText);
		        exit (-1);
	        }
            else
		        EF_fprintf (stderr, myT ("错误(%d): %s\n"), nErrorCode, szErrorText);
        }
    }
    else
    {
        if (EF_IsEmptyStr (szErrorText) == EF_TRUE)
        {
	        if (nErrorCode < 0)
	        {
		        EF_fprintf (stderr, myT ("类库\"%s\"中发生错误码为%d的严重错误\n"), szClsLibName, nErrorCode);
		        exit (-1);
	        }
            else
		        EF_fprintf (stderr, myT ("类库\"%s\"中发生错误码为%d的错误\n"), szClsLibName, nErrorCode);
        }
        else
        {
	        if (nErrorCode < 0)
	        {
		        EF_fprintf (stderr, myT ("类库\"%s\"中发生严重错误(%d): %s\n"), szClsLibName, nErrorCode, szErrorText);
		        exit (-1);
	        }
            else
		        EF_fprintf (stderr, myT ("类库\"%s\"中发生错误(%d): %s\n"), szClsLibName, nErrorCode, szErrorText);
        }
    }
}

// 返回所处常量区首地址。
// 使用本函数之前需要先锁定常量区表
static EFConstDataHeader* _EF_IsConstData (void* pPtr)
{
    if (ef_pRuntimeClassInfo == NULL)
    {
        if (pPtr >= &g_EmptyConst && pPtr < (EFByte*)&g_EmptyConst + g_EmptyConst.m_header.m_nConstDataSize)
            return (EFConstDataHeader*)&g_EmptyConst;
    }
    else
    {
        EFConstDataHeader* pd = (EFConstDataHeader*)&g_EmptyConst;
        while (pd != NULL)
        {
            if (pPtr >= pd && pPtr < (EFByte*)pd + pd->m_nConstDataSize)
                return pd;
            pd = pd->m_prev;
        }

        pd = g_EmptyConst.m_header.m_next;
        while (pd != NULL)
        {
            if (pPtr >= pd && pPtr < (EFByte*)pd + pd->m_nConstDataSize)
                return pd;
            pd = pd->m_next;
        }
    }

    return NULL;
}

bool EF_IsEFConstData (void* pPtr)
{
    if (ef_pRuntimeClassInfo != NULL)
        sys_Runtime_LockConstDataInfTab ();

    EFConstDataHeader* pd = _EF_IsConstData (pPtr);

    if (ef_pRuntimeClassInfo != NULL)
        sys_Runtime_UnlockConstDataInfTab ();

    return pd != NULL;
}

void* EF_malloc (int nReqMemSize)
{
	if (nReqMemSize == 0)
		return NULL;

	void* p;

#ifdef WIN32
	p = ::HeapAlloc (::GetProcessHeap (), 0, nReqMemSize);
#else
	p = malloc (nReqMemSize);
#endif

	if (p == NULL)
		EF_ReportEFRuntimeError (myT (""), RTERR_F_SYS_MALLOC, myT ("分配内存失败"));
	return p;
}

void* EF_realloc (void* pPtr, int nReallocMemSize)
{
	if (nReallocMemSize == 0)
	{
		EF_free (pPtr);
		return NULL;
	}

	if (pPtr == NULL)
		return EF_malloc (nReallocMemSize);

	//////////////////////////////////////////////////

	if (ef_pRuntimeClassInfo != NULL)
		sys_Runtime_LockConstDataInfTab ();

	EFConstDataHeader* pd = _EF_IsConstData (pPtr);

	void* pnew;
	if (pd != NULL)
	{
		pnew = EF_malloc (nReallocMemSize);
		memcpy (pnew, pPtr, EF_min (((EFByte*)pd + pd->m_nConstDataSize) - (EFByte*)pPtr, nReallocMemSize));
		EF_free (pPtr);
	}
	else
	{
#ifdef WIN32
		pnew = ::HeapReAlloc (::GetProcessHeap (), 0, pPtr, nReallocMemSize);
#else
		pnew = realloc (pPtr, nReallocMemSize);
#endif
		if (pnew == NULL)
			EF_ReportEFRuntimeError (myT (""), RTERR_F_SYS_MALLOC, myT ("分配内存失败"));
	}

	if (ef_pRuntimeClassInfo != NULL)
		sys_Runtime_UnlockConstDataInfTab ();

	return pnew;
}

void EF_free (void* pPtr)
{
	if (pPtr != NULL && EF_IsEFConstData (pPtr) == false)
	{
#ifdef WIN32
		::HeapFree (::GetProcessHeap (), 0, pPtr);
#else
		free (pPtr);
#endif
	}
}

//!!! 必须保持本算法不能更改，否则会影响到程序其它位置。
EFDWord EF_GetStrHash (EFChar* k)
{
    if (k == NULL)
        return 0;

    EFDWord hash = 0;
    while (*k)
        hash += *k++;

    return hash;
}

//!!! 必须保持本算法不能更改，否则会影响到程序其它位置。
EFDWord EF_GetStrHash (EFChar* k, int nLen)
{
    ASSERT (nLen >= 0);
    if (k == NULL)
        return 0;

    EFDWord hash = 0;
    while (nLen > 0 && *k)
    {
        hash += *k++;
        nLen--;
    }

    return hash;
}

//!!! 必须保持本算法不能更改，否则会影响到程序其它位置。
EFDWord EF_GetFriendlyNameHash (EFChar* k)
{
    if (k == NULL)
        return 0;

    EFDWord hash = 0;
    while (*k)
    {
        if (*k == myC('<'))
        {
            k++;
            while (*k && *k != myC('>'))
                k++;
            if (*k == myC('\0'))
                break;

            ASSERT (*k == myC('>'));
            k++;
        }
        else
        {
            hash += *k;

            if ((EFDWord)*k <= myC(' '))  // 将多个空白合并成一个.
            {
                k++;
                while (*k != myC('\0') && (EFDWord)*k <= myC(' '))
                    k++;
            }
            else
                k++;
        }
    }

    return hash;
}

//!!! 必须保持本算法不能更改，否则会影响到程序其它位置。
EFDWord EF_GetFriendlyNameHash (EFChar* k, int nLen)
{
    ASSERT (nLen >= 0);
    if (k == NULL)
        return 0;

    EFDWord hash = 0;
    while (nLen > 0 && *k)
    {
        if (*k == myC('<'))
        {
            k++;
            nLen--;
            while (nLen > 0 && *k && *k != myC('>'))
            {
                k++;
                nLen--;
            }
            if (nLen == 0 || *k == myC('\0'))
                break;

            ASSERT (*k == myC('>'));
            k++;
            nLen--;
        }
        else
        {
            hash += *k;

            if ((EFDWord)*k <= myC(' '))  // 将多个空白合并成一个.
            {
                k++;
                nLen--;
                while (nLen > 0 && (EFDWord)*k <= myC(' '))
                {
                    k++;
                    nLen--;
                }
            }
            else
            {
                k++;
                nLen--;
            }
        }
    }

    return hash;
}

bool EF_IsSameFriendlyName (EFChar* szFriendName1, EFDWord dwFriendNameHashValue1,
                       EFChar* szFriendName2, EFDWord dwFriendNameHashValue2, bool blIsMethodFriendName)
{
    return dwFriendNameHashValue1 == dwFriendNameHashValue2 &&
            EF_IsSameFriendlyName (szFriendName1, szFriendName2, blIsMethodFriendName);
}

bool EF_IsSameFriendlyName (EFChar* szFriendName1, EFChar* szFriendName2, bool blIsMethodFriendName)
{
    if (szFriendName1 == NULL)  szFriendName1 = myT ("");
    if (szFriendName2 == NULL)  szFriendName2 = myT ("");

	while (true)
    {
        EFChar ch = *szFriendName1++;
        if (ch != *szFriendName2++)
            return false;

        if (ch == myC('\0'))
            return true;

        if (ch == myC(' '))  // 忽略不一致空格的数目
        {
            while (*szFriendName1 == myC(' '))
                szFriendName1++;
            while (*szFriendName2 == myC(' '))
                szFriendName2++;
        }
        else if (blIsMethodFriendName == false && ch == myC('<'))
              // 对于方法友好名称，由于<>中包含的是参数索引位置，所以必须全部比较。
        {
            while (*szFriendName1 != myC('\0') && *szFriendName1 != myC('>'))
                szFriendName1++;

            while (*szFriendName2 != myC('\0') && *szFriendName2 != myC('>'))
                szFriendName2++;

            if (*szFriendName1 == myC('\0') || *szFriendName2 == myC('\0'))
            {
                FAIL;
                return false;
            }

            szFriendName1++;
            szFriendName2++;
        }
    }
}

bool EF_IsSameStr (EFChar* szText1, EFDWord dwHashValue1, EFChar* szText2, EFDWord dwHashValue2)
{
    if (dwHashValue1 != dwHashValue2)
        return false;
    else
        return EF_strcmp (szText1, szText2) == 0;
}

typedef struct _tag_EF_SysDataTypeInf
{
    EF_DatatypeType m_enType;
    EFChar* m_szDataType;
    EFDWord m_dwDataTypeHash;
    EFChar* m_szName;
}
_EF_SysDataTypeInf;

static _EF_SysDataTypeInf s_inf [] =
{
    {  EF_DT_INT,        EF_INT,     EF_GetStrHash (EF_INT),    EF_NAME_INT         },
    {  EF_DT_DOUBLE,     EF_DOUBLE,  EF_GetStrHash (EF_DOUBLE), EF_NAME_DOUBLE      },
    {  EF_DT_BOOL,       EF_BOOL,    EF_GetStrHash (EF_BOOL),   EF_NAME_BOOL        },
    {  EF_DT_TEXT,       EF_TEXT,    EF_GetStrHash (EF_TEXT),   EF_NAME_TEXT        },
    {  EF_DT_BIN,        EF_BIN,     EF_GetStrHash (EF_BIN),    EF_NAME_BIN         },
    {  EF_DT_INT_ARRAY,    EF_ARRAY_OF (EF_INT),    EF_GetStrHash (EF_ARRAY_OF (EF_INT)),    myT("整数数组")   },
    {  EF_DT_DOUBLE_ARRAY, EF_ARRAY_OF (EF_DOUBLE), EF_GetStrHash (EF_ARRAY_OF (EF_DOUBLE)), myT("小数数组")   },
    {  EF_DT_BOOL_ARRAY,   EF_ARRAY_OF (EF_BOOL),   EF_GetStrHash (EF_ARRAY_OF (EF_BOOL)),   myT("逻辑型数组") },
    {  EF_DT_TEXT_ARRAY,   EF_ARRAY_OF (EF_TEXT),   EF_GetStrHash (EF_ARRAY_OF (EF_TEXT)),   myT("文本数组")   },
    {  EF_DT_BIN_ARRAY,    EF_ARRAY_OF (EF_BIN),    EF_GetStrHash (EF_ARRAY_OF (EF_BIN)),    myT("字节集数组") }
};

//not use?
void _EF_FillBaseTypeInfo (EF_DatatypeType enBaseType, EFDataTypeInfo* pDataTypeInfo)
{
    ASSERT (pDataTypeInfo != NULL);

    if (enBaseType != EF_DT_NULL)
    {
        for (int i = 0; i < sizeof (s_inf) / sizeof (s_inf [0]); i++)
        {
            if (s_inf [i].m_enType == enBaseType)
            {
                pDataTypeInfo->m_enDataType = enBaseType;
                pDataTypeInfo->m_szDataType = s_inf [i].m_szDataType;
                pDataTypeInfo->m_dwDataTypeHash = s_inf [i].m_dwDataTypeHash;
                pDataTypeInfo->m_pTypeInfo = NULL;
                return;
            }
        }

        FAIL;  // 如果到这里说明提供的enBaseType有问题。
    }

    pDataTypeInfo->m_enDataType = EF_DT_NULL;
}

EF_CString EF_GetDatatypeDisplayNameString (EFChar* szDataType, EFDWord dwDataTypeHash)
{
    if (EF_IsEmptyStr (szDataType) == EF_TRUE)
        return EF_CString (EF_NAME_NULL);

    if (EF_IS_BASE_DATATYPE_SZ (szDataType))
    {
        for (int i = 0; i < sizeof (s_inf) / sizeof (s_inf [0]); i++)
        {
            if (s_inf [i].m_dwDataTypeHash == dwDataTypeHash &&
                    EF_strcmp (s_inf [i].m_szDataType, szDataType) == 0)
                return EF_CString (s_inf [i].m_szName);
        }

        FAIL;
    }
    else if (*szDataType == EF_ARRAY_DATA_TYPE_LEADER_CHAR)
    {
        return EF_CString (szDataType + 1) + myT ("[]");
    }

    return EF_CString (szDataType);
}

EFChar* EF_GetDatatypeTypeDisplayName (EF_DatatypeType enType)
{
    if (enType == EF_DT_CLASS_INTERFACE_ENUM)
        return myT ("对象/接口/枚举");
    if (enType == EF_DT_CLASS_INTERFACE_ENUM_ARRAY)
        return myT ("对象/接口/枚举数组");

    for (int i = 0; i < sizeof (s_inf) / sizeof (s_inf [0]); i++)
    {
        if (s_inf [i].m_enType == enType)
            return s_inf [i].m_szName;
    }

    return myT ("未知类型");
}

EF_DatatypeType EF_GetDatatypeType (EFChar* szDataType, EFDWord dwDataTypeHash)
{
    if (EF_IsEmptyStr (szDataType) == EF_TRUE)
        return EF_DT_NULL;

    if (dwDataTypeHash == 0)
        dwDataTypeHash = EF_GetStrHash (szDataType);

    if (EF_IS_BASE_DATATYPE_SZ (szDataType))
    {
        for (int i = 0; i < sizeof (s_inf) / sizeof (s_inf [0]); i++)
        {
            if (s_inf [i].m_dwDataTypeHash == dwDataTypeHash &&
                    EF_strcmp (s_inf [i].m_szDataType, szDataType) == 0)
                return s_inf [i].m_enType;
        }

        FAIL;
        return EF_DT_NULL;
    }
    else
    {
        return szDataType [0] == EF_ARRAY_DATA_TYPE_LEADER_CHAR ? EF_DT_CLASS_INTERFACE_ENUM_ARRAY :
                EF_DT_CLASS_INTERFACE_ENUM;
    }
}

EFArray EF_CloneEFArray (EFArray pAryInf, EF_DatatypeType dtType)
{
	if (pAryInf == NULL)
		return EF_EMPTY_ARRAY;

	EFByte* pAryData;
	int nElementCount = EF_GetEFArraySize (pAryInf, (void**)&pAryData);
	int nDataSize = EF_GetDatatypeSizeOfArrayElement (dtType) * nElementCount;

	int nHeadSize = pAryData - (EFByte*)pAryInf;
	EFArray aryNew = (EFArray)EF_malloc (nHeadSize + nDataSize);
	memcpy (aryNew, pAryInf, nHeadSize);
	*(EFInt*)aryNew = 0;

	if (nElementCount > 0)
	{
		int i;
		EFByte* pNewAryData = (EFByte*)aryNew + nHeadSize;

		switch (dtType)
		{
		case EF_DT_INT_ARRAY:
		case EF_DT_DOUBLE_ARRAY:
		case EF_DT_BOOL_ARRAY:
			memcpy (pNewAryData, pAryData, nDataSize);
			break;

		case EF_DT_TEXT_ARRAY:
			ASSERT (sizeof (EFText) == sizeof (EFInt));
			for (i = 0; i < nElementCount; i++)
			{
                EFText text = EF_CloneEFText (*(EFText*)pAryData);
				*(EFText*)pNewAryData = text;
                EF_GC_REG_TEXT_DATA (text);
				pNewAryData += sizeof (EFText);
				pAryData += sizeof (EFText);
			}
			break;

		case EF_DT_BIN_ARRAY:
			ASSERT (sizeof (EFBinary) == sizeof (EFInt));
			ASSERT (sizeof (EFText) == sizeof (EFInt));
			for (i = 0; i < nElementCount; i++)
			{
                EFBinary bin = EF_CloneEFBin (*(EFBinary*)pAryData);
				*(EFBinary*)pNewAryData = bin;
                EF_GC_REG_BIN_DATA (bin);
				pNewAryData += sizeof (EFBinary);
				pAryData += sizeof (EFBinary);
			}
			break;

		default:
			memset (pNewAryData, 0, nDataSize);
			break;
		}
	}

	return aryNew;
}

//获取易语言类库搜索路径，结果写入参数paths中。
void EF_GetEFLibSearchPaths(EF_CStringArray& paths)
{
	EF_CString strEnv, strPath;
	strEnv.SetText (getenv (EF_LIB_PATHS_NAME));

	EFChar* szToken = EF_strtok (strEnv.GetText (), myT(",;"));
	while (szToken != NULL)
	{
		strPath = szToken;
		if (strPath.IsEmpty () == EF_FALSE)
		{
			if (strPath.EndOf (EF_PATH_CHAR_TEXT) == EF_FALSE)
				strPath += EF_PATH_CHAR_TEXT;
			paths.Add_NoClone (strPath.Detach ());
		}
		szToken = EF_strtok (NULL, myT(",;"));
	}
}

// 返回pb数据的校验和
EFDWord EF_GetDataSum (EFByte* pb, int nDataSize)
{
    EFDWord dwSumCode = 0;

    int nDWordCount = nDataSize / sizeof (EFDWord);
    for (int i = 0; i < nDWordCount; i++)
    {
        dwSumCode += *(EFDWord*)pb;
        pb += sizeof (EFDWord);
        nDataSize -= sizeof (EFDWord);
    }

    ASSERT (nDataSize < sizeof (EFDWord));
    EFDWord dw = 0;
    memcpy (&dw, pb, nDataSize);
    return dwSumCode + dw;
}

void* EF_GetEFObjectDataByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex)
{
	ASSERT (nClassIndex >= 0 && nClassIndex < pObject->m_pObjectInfo->m_nClassCount);
	ASSERT (pObject->m_paryData [nClassIndex] != NULL);

    return (void*)EF_GetIntByIndex (pObject->m_paryData [nClassIndex], nItemIndex);
}

void EF_SetEFObjectDataByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex, void* value)
{
	ASSERT (nClassIndex >= 0 && nClassIndex < pObject->m_pObjectInfo->m_nClassCount);
	ASSERT (pObject->m_paryData [nClassIndex] != NULL);

    EF_SetIntByIndex (pObject->m_paryData [nClassIndex], nItemIndex, (EFInt)(intptr_t)value);
}

//注意索引仍然是以EFInt为单位的索引
EFDouble EF_GetDoubleFieldOfEFObjectByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex)
{
	ASSERT (nClassIndex >= 0 && nClassIndex < pObject->m_pObjectInfo->m_nClassCount);
	ASSERT (pObject->m_paryData [nClassIndex] != NULL);

	return *(EFDouble*)EF_GetAddressByIntIndex (pObject->m_paryData [nClassIndex], nItemIndex);
}

void EF_SetDoubleFieldOfEFObjectByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex, EFDouble dblValue)
{
	ASSERT (nClassIndex >= 0 && nClassIndex < pObject->m_pObjectInfo->m_nClassCount);
	ASSERT (pObject->m_paryData [nClassIndex] != NULL);

	*(EFDouble*)EF_GetAddressByIntIndex (pObject->m_paryData [nClassIndex], nItemIndex) = dblValue;
}

void EF_CloneIntoEFTextVar (EFText* ppTextVar, EFText new_text)
{
    ASSERT (ppTextVar != NULL);

    if (new_text == NULL)
        new_text = EF_EMPTY_TEXT;

	EF_DEC_REF_COUNT (*ppTextVar);
    *ppTextVar = (EFText)EF_INC_REF_COUNT (new_text);
}

void EF_CloneIntoEFBinVar (EFBinary* ppBinVar, EFBinary new_bin)
{
    ASSERT (ppBinVar != NULL);

    if (new_bin == NULL)
        new_bin = EF_EMPTY_BIN;

	EF_DEC_REF_COUNT (*ppBinVar);
    *ppBinVar = (EFBinary)EF_INC_REF_COUNT (new_bin);
}

void EF_CloneIntoEFObjectVar (EFObject** ppObjectVar, EFObject* new_object)
{
    ASSERT (ppObjectVar != NULL);

	EF_DEC_REF_COUNT (*ppObjectVar);
    *ppObjectVar = (EFObject*)EF_INC_REF_COUNT (new_object);
}

void EF_CloneIntoEFArrayVar (EFArray* ppArrayVar, EFArray new_array)
{
    ASSERT (ppArrayVar != NULL);

    if (new_array == NULL)
        new_array = EF_EMPTY_ARRAY;

	EF_DEC_REF_COUNT (*ppArrayVar);
    *ppArrayVar = (EFArray)EF_INC_REF_COUNT (new_array);
}

// 取类名最后一个“.”或最后一个“/”后面的文本，并去除最后的“事件”二字。
// “用户界面.被单击事件” -> “被单击”
// “用户界面.按钮/被单击事件” -> “被单击”
// 特殊地，“系统.事件” -> “事件”，并不去除最后的“事件”二字，因为去除后就没有名称了。
EF_CString EF_GetEFEventShortName (EFChar* szEventClassName)
{
	EFChar* szLastDot = EF_strrchr (szEventClassName, myC('.'));
	EFChar* szLastSlash = EF_strrchr (szEventClassName, myC('/'));

	EFChar* szEventShortName = szEventClassName;
	if(szLastDot)   szEventShortName = szLastDot + 1;
	if(szLastSlash) szEventShortName = szLastSlash + 1;

    EF_CString strShortName = szEventShortName;

    static EFChar* s_szEventName = myT ("事件");
	if (strShortName.EndOf (s_szEventName))
		strShortName = strShortName.Left (strShortName.GetLength () - EF_strlen (s_szEventName));

	return strShortName;
}

EF_CString EF_GetEFEventShortName (EFObject* pEventObject)
{
	ASSERT (pEventObject != NULL);
	return EF_GetEFEventShortName (EF_GetClassNameOfEFObject (pEventObject));
}

EFChar* EF_GetClassNameOfEFObject (EFObject* pObject)
{
	ASSERT (pObject != NULL);
	return EF_GET_CLASS_INFO (pObject)->m_szName;
}

// 如果改动EF_IsNameLeaderChar和EF_IsNameNextChar这两个函数，必须同步更新LexEF.cpp中的对应函数。
bool EF_IsNameLeaderChar (EFChar ch)
{
    return (unsigned int)ch >= 128 ||
        (unsigned int)ch >= myC('A') && (unsigned int)ch <= myC('Z') ||
        (unsigned int)ch >= myC('a') && (unsigned int)ch <= myC('z') ||
        ch == myC('_');
}

bool EF_IsNameNextChar (EFChar ch)
{
    return (unsigned int)ch >= myC('0') && (unsigned int)ch <= myC('9') || EF_IsNameLeaderChar (ch);
}

EFChar* EF_SkipName (EFChar* ps)
{
    ASSERT (ps != NULL);

    while (true)
    {
        if (EF_IsNameNextChar (*ps) == false)
            return ps;
        ps++;
    }
}

bool EF_IsValidName (EFChar* szName, bool blAllowInnerMidChar, bool blAllowNameHideChar)
{
    ASSERT (szName != NULL);

    EFChar ch = *szName++;
    if (EF_IsNameLeaderChar (ch) || blAllowNameHideChar && ch == EF_HIDDEN_NAME_DELIMITER_CHAR)
    {
        while (true)
        {
            ch = *szName++;
            if (ch == myC('\0'))
                return true;

            if (EF_IsNameNextChar (ch) == false &&
                    (blAllowInnerMidChar == false || ch != EF_INNER_NAME_DELIMITER_CHAR) &&  // EF_INNER_NAME_DELIMITER_CHAR用来分隔嵌入类型和嵌入方法名称
                    (blAllowNameHideChar == false || ch != EF_HIDDEN_NAME_DELIMITER_CHAR))
                return false;
        }
    }

    return false;
}

// nMethodArgCount不等于-1表示szFriendName为类方法的友好名称，否则为全局友好名称。
bool EF_IsValidFriendlyName (EFChar* szFriendName, int nMethodArgCount)
{
    if (EF_IsEmptyStr (szFriendName))
        return false;

    EF_IntArray naryArgIndexRecord;
	while (true)
    {
        EFChar ch = *szFriendName++;

        if (ch == myC('\0'))
        {
            if (nMethodArgCount != -1)
                return naryArgIndexRecord.GetCount () == nMethodArgCount;
            else
                return true;
        }

        if (ch == myC('\''))
            return false;

        if (ch == myC('<'))
        {
            ch = *szFriendName++;

            if (nMethodArgCount != -1)
            {
                ASSERT (nMethodArgCount >= 0);

                if (ch < myC('0') || ch > myC('9'))
                    return false;

                int n = ch - myC('0');
                while (true)
                {
                    ch = *szFriendName++;
                    if (ch == myC('>'))
                        break;

                    if (ch < myC('0') || ch > myC('9'))
                        return false;
                    n = n * 10 + (ch - myC('0'));
                }

                if (n < 1 || n > nMethodArgCount || naryArgIndexRecord.Find (n) != -1)
                    return false;

                naryArgIndexRecord.Add (n);
            }
            else
            {
                if (EF_IsNameLeaderChar (ch) == false)
                    return false;

                while (true)
                {
                    ch = *szFriendName++;
                    if (ch == myC('>'))
                        break;

                    if (EF_IsNameNextChar (ch) == false)
                        return false;
                }
            }
        }
    }
}

// blAllowCommaCompart为真表示允许组合数据类型, 类似: "int, TextFile"，组合数据类型只有在方法的返回值定义中才能使用。
bool EF_IsValidDataTypeName (EFChar* szDataTypeName, bool blAllowCommaCompart)
{
    if (EF_IsEmptyStr (szDataTypeName) == EF_TRUE)
        return false;

    while (true)
    {
        EFChar ch = *szDataTypeName++;
        if (ch == EF_ARRAY_DATA_TYPE_LEADER_CHAR)
            ch = *szDataTypeName++;

        if (EF_IsNameLeaderChar (ch) == false)
            break;

        while (true)
        {
            ch = *szDataTypeName++;
            if (ch == myC('\0'))
                return true;

            if (blAllowCommaCompart && ch == myC(','))
            {
                while (*szDataTypeName == myC(' '))
                    szDataTypeName++;
                break;
            }

            if (ch == myC('.'))
                break;

            if (EF_IsNameNextChar (ch) == false && ch != EF_INNER_NAME_DELIMITER_CHAR)  // EF_INNER_NAME_DELIMITER_CHAR用来分隔嵌入类型
                return false;
        }
    }

    return false;
}

//   与上函数的区别是：EF_IsValidDataTypeName判断的是数据类型名称的有效性，
// 本函数判断的是类型本身(接口/类/枚举)的全名或省略名称的有效性。
bool EF_IsValidTypeName (EFChar* szName)
{
    if (EF_IsEmptyStr (szName) == EF_TRUE)
        return false;

    while (true)
    {
        EFChar ch = *szName++;
        if (EF_IsNameLeaderChar (ch) == false)
            break;

        while (true)
        {
            ch = *szName++;
            if (ch == myC('\0'))
                return true;

            if (ch == myC('.'))
                break;

            if (EF_IsNameNextChar (ch) == false && ch != EF_INNER_NAME_DELIMITER_CHAR)  // EF_INNER_NAME_DELIMITER_CHAR用来分隔嵌入类型
                return false;
        }
    }

    return false;
}

// 判断是否为组合数据类型, 类似: "int, TextFile"
bool EF_IsCmbDatatype (EFChar* szDataType)
{
    return EF_strchr (szDataType, myC(',')) != NULL;
}

EFChar* EF_GetFileNamePart (EFChar* szPath)
{
	EFChar* ps = EF_strrchr (szPath, EF_PATH_CHAR);
	if (ps != NULL)
        return ps + 1;
    else
        return szPath;
}

// 所返回的路径以EF_PATH_CHAR结束。
bool EF_GetPathPart (EFChar* szFullFileName, EF_CString& strPath)
{
    ASSERT (szFullFileName != NULL);

	EFChar* ps = EF_strrchr (szFullFileName, EF_PATH_CHAR);
	if (ps == NULL)
    {
        strPath.Empty ();
        return false;
    }
    else
    {
        strPath.SetText (szFullFileName, ps - szFullFileName + 1);
        return true;
    }
}

void EF_SplitStrings (EFChar* ps, EF_CStringArray& strary)
{
    strary.RemoveAll ();

    // 由于EF_strtok将修改ps所指向的内容，所以此处需要复制。
    EF_CString strCopy (ps);
    ps = strCopy.GetText ();

    EF_CString str;
	EFChar* szToken = EF_strtok (ps, myT(","));
	while (szToken != NULL)
	{
		str = szToken;
        str.TrimLeft ();
        str.TrimRight ();

        if (str.IsEmpty () == EF_FALSE)
            strary.Add_NoClone (str.Detach ());

		szToken = EF_strtok (NULL, myT(","));
	}
}

// 将szEFormatText中包含转义符的文本转换成纯粹的文本.
// szEFormatText中允许包含以下转义符号：\t \r \n \\ \" \' \x????('?'为任意十六进制数字)
EFChar* EF_ConvertEscapeSequences (EFChar* szEFormatText, int nLen, EF_MiniMem& memBuf)
{
    ASSERT (nLen >= -1);

    memBuf.Empty ();

    if (nLen == -1)
        nLen = EF_strlen (szEFormatText);

    if (nLen == 0)
        return myT ("");

    /////////////////////////////////////////////////

    EFChar ch;
    EFChar* ps = szEFormatText;
    EFChar* pb = ps;

    while (nLen > 0 && *ps != myC('\0'))
    {
        ch = *ps++;
        nLen--;
        if (ch != myC('\\'))
            continue;

        if (nLen == 0)
            break;

        EFChar* ps2 = ps - 1;
        ch = *ps++;
        nLen--;

        bool blModi = true;
        switch (ch)
        {
        case myC('t'):
            ch = myC('\t');
            break;
        case myC('r'):
            ch = myC('\r');
            break;
        case myC('n'):
            ch = myC('\n');
            break;
        case myC('\\'):
            ch = myC('\\');
            break;
        case myC('\"'):
            ch = myC('\"');
            break;
        case myC('\''):
            ch = myC('\'');
            break;
        case myC('x'):  {
            ch = 0;
            for (int i = 0; nLen > 0 && i < sizeof (EFChar) * 2; i++)
            {
                EFChar ch2 = *ps;
                if (ch2 >= myC('0') && ch2 <= myC('9'))
                {
                    ch = (ch << 4) + (ch2 - myC('0'));
                }
                else if (ch2 >= myC('a') && ch2 <= myC('f'))
                {
                    ch = (ch << 4) + (ch2 - myC('a') + 10);
                }
                else if (ch2 >= myC('A') && ch2 <= myC('F'))
                {
                    ch = (ch << 4) + (ch2 - myC('A') + 10);
                }
                else
                    break;
                ps++;
                nLen--;
            }
            break;  }
        default:
            blModi = false;
            break;
        }

        if (blModi)
        {
            if (ps2 > pb)
                memBuf.Append ((EFByte*)pb, (ps2 - pb) * sizeof (EFChar));
            memBuf.AddChar (ch);
            pb = ps;
        }
    }

    if (ps > pb)
        memBuf.Append ((EFByte*)pb, (ps - pb) * sizeof (EFChar));

    memBuf.AddChar (myC('\0'));
    return (EFChar*)memBuf.GetPtr ();
}

/* 1. The format of the text pointted by szExtendPropertiesAggregate must like as "name name=value ...".
   2. The value's type can be boolean, text or number.
   3. Number value may be hexadecimal(leads by 0X), binary(0B) or decimalist format, For example:
        length = 0x123
        length = 0b10101
   4. Character combinations consisting of a backslash (\) followed by a letter or by a combination of digits
   are called "escape sequences." Following be supported:
       \t  \r  \n  \\  \"  \'  \x????('?' is a anyone hexadecimal character)
     These escape sequences can be used in a property's text value, and will be converted automatic, For example:
       action = "call\tme"
       character = "\""
   5. The function returns true if specified property has be found, returns false if not found or the text format is wrong.
   6. More explanation see the remark about "m_szExtendedPropertyList" in the file "ecl.h".
*/
bool EF_GetExtendedPropertyValue (EFChar* szExtendPropertiesAggregate, EFChar* szFindPropertyName,
                             EF_ExtendedPropertyValue& value, EF_MiniMem& memBuf)
{
    ASSERT (szFindPropertyName != NULL);

    if (EF_IsEmptyStr (szExtendPropertiesAggregate) == EF_TRUE)
        return false;

    static int s_nTrueTextLen = EF_strlen (EF_NAME_TRUE);
    static int s_nFalseTextLen = EF_strlen (EF_NAME_FALSE);
    static int s_nETrueTextLen = EF_strlen (EF_NAME_TRUE_E);
    static int s_nEFalseTextLen = EF_strlen (EF_NAME_FALSE_E);

    int nPropertyNameLen = EF_strlen (szFindPropertyName);
    if (nPropertyNameLen == 0)
        return false;

    EFChar* ps = szExtendPropertiesAggregate;
    while (true)
    {
        EFChar ch = *ps;
        if (ch == myC('\0'))
            break;

        if ((EFDWord)ch <= (EFDWord)myC(' '))  // Skips space
        {
            ps++;
            continue;
        }

        if (EF_IsNameLeaderChar (ch) == false)  // First must is name.
            break;

        EFChar* ps2 = EF_SkipName (ps);
        ASSERT (ps2 > ps);
        bool blFound = (nPropertyNameLen == ps2 - ps &&
                EF_strncmp (ps, szFindPropertyName, nPropertyNameLen ) == 0);  // Has found?
        ps = ps2;

        ////////////////////////////////////////////////

        // Skips spaces.
        while (*ps != myC('\0') && (EFDWord)*ps <= (EFDWord)myC(' '))
            ps++;

        if (*ps != myC('='))  // Is value setted?
        {
            if (blFound)  // If no value be setted, default value is boolean true.
            {
                value.m_enType = EF_EPT_BOOL;
                value.u.m_bool = EF_TRUE;
                return true;
            }
            else
                continue;
        }

        ps++; // Skips the '=' character.

        // Skips spaces.
        while (*ps != myC('\0') && (EFDWord)*ps <= (EFDWord)myC(' '))
            ps++;

        ////////////////////////////////////////////////

        // Begin splitting property value.
        ch = *ps;

        if ((EFDWord)ch >= (EFDWord)myC('0') && (EFDWord)ch <= (EFDWord)myC('9') ||
                ch == myC('+') || ch == myC('-'))  // Is number?
        {
            if (ch == myC('0') && (*(ps + 1) == myC('X') || *(ps + 1) == myC('x')))  // Is hexadecimal value?
            {
                // Skips "0X"
                ps++;
                ps++;

                EFInt nValue = 0;
                while (true)
                {
                    ch = *ps;

                    if ((EFDWord)ch >= (EFDWord)myC('0') && (EFDWord)ch <= (EFDWord)myC('9'))
                    {
                        if (blFound)
                            nValue = (nValue << 4) + (ch - myC('0'));
                    }
                    else if ((EFDWord)ch >= (EFDWord)myC('a') && (EFDWord)ch <= (EFDWord)myC('f'))
                    {
                        if (blFound)
                            nValue = (nValue << 4) + (ch - myC('a') + 10);
                    }
                    else if ((EFDWord)ch >= myC('A') && (EFDWord)ch <= myC('F'))
                    {
                        if (blFound)
                            nValue = (nValue << 4) + (ch - myC('A') + 10);
                    }
                    else if ((EFDWord)ch > (EFDWord)myC(' '))  // It's invalid if not space and hexadecimal number character be found.
                    {
                        return false;
                    }
                    else
                        break;  // If space character be arrived, indicate it's end of this hexadecimal value.

                    ps++;
                }

                if (blFound)
                    value.u.m_double = (EFDouble)nValue;
            }
            else if (ch == myC('0') && (*(ps + 1) == myC('B') || *(ps + 1) == myC('b')))  // Is binary value?
            {
                // Skips "0B"
                ps++;
                ps++;

                EFInt nValue = 0;
                while (true)
                {
                    ch = *ps;

                    if (ch == myC('0') || ch == myC('1'))
                    {
                        if (blFound)
                            nValue = (nValue << 1) + (*ps - myC('0'));
                    }
                    else if ((EFDWord)ch > (EFDWord)myC(' '))  // Alike cause as front.
                    {
                        return false;
                    }
                    else
                        break;

                    ps++;
                }

                if (blFound)
                    value.u.m_double = (EFDouble)nValue;
            }
            else  // Otherwise, it's a decimalist value.
            {
                ps2 = ps;  // Remember the start position of this text.

                if (ch == myC('+') || ch == myC('-'))  // Skips the sign character.
                    ps++;

                bool blFoundDot = false;
                bool blFoundE = false;
                while (true)
                {
                    ch = *ps;
                    if ((EFDWord)ch >= (EFDWord)myC('0') && (EFDWord)ch <= (EFDWord)myC('9'))
                    {
                        ps++;
                    }
                    else if (ch == myC('.'))
                    {
                        if (blFoundDot || blFoundE)
                            return false;
                        blFoundDot = true;
                        ps++;
                    }
                    else if (ch == myC('d') || ch == myC('D') || ch == myC('e') || ch == myC('E'))
                    {
                        if (blFoundE)
                            return false;
                        blFoundE = true;
                        ps++;
                        if (*ps == myC('+') || *ps == myC('-'))
                            ps++;
                    }
                    else if ((EFDWord)ch > (EFDWord)myC(' '))
                    {
                        return false;
                    }
                    else
                        break;
                }

                if (blFound)
                {
                    EF_CString str (ps2, ps - ps2);
                    EF_MiniMem mem;
                    value.u.m_double = (EFDouble)atof (EF_GetMBSStr (str.GetText (), mem));
                }
            }

            if (blFound)
            {
                value.m_enType = EF_EPT_NUM;
                return true;
            }
        }
        else if (EF_strncmp (ps, EF_NAME_TRUE, s_nTrueTextLen) == 0)  // Is boolean true?
        {
            if (blFound)
            {
                value.m_enType = EF_EPT_BOOL;
                value.u.m_bool = EF_TRUE;
                return true;
            }
            else
                ps += s_nTrueTextLen;
        }
        else if (EF_strncmp (ps, EF_NAME_FALSE, s_nFalseTextLen) == 0)  // Is boolean false?
        {
            if (blFound)
            {
                value.m_enType = EF_EPT_BOOL;
                value.u.m_bool = EF_FALSE;
                return true;
            }
            else
                ps += s_nFalseTextLen;
        }
        else if (EF_strncmp (ps, EF_NAME_TRUE_E, s_nETrueTextLen) == 0)  // Is boolean true?
        {
            if (blFound)
            {
                value.m_enType = EF_EPT_BOOL;
                value.u.m_bool = EF_TRUE;
                return true;
            }
            else
                ps += s_nETrueTextLen;
        }
        else if (EF_strncmp (ps, EF_NAME_FALSE_E, s_nEFalseTextLen) == 0)  // Is boolean false?
        {
            if (blFound)
            {
                value.m_enType = EF_EPT_BOOL;
                value.u.m_bool = EF_FALSE;
                return true;
            }
            else
                ps += s_nEFalseTextLen;
        }
        else if (ch == myC('\"'))  // Is string?
        {
            ps++;
            ps2 = ps;

            while (true)
            {
                ch = *ps;
                if (ch == myC('\0'))
                    return false;

                if (ch == myC('\"'))
                    break;

                ps++;
                if (ch == myC('\\') && (*ps == myC('\\') || *ps == myC('\"')))  // Skips \\ and \" .
                    ps++;
            }

            if (blFound)
            {
                value.m_enType = EF_EPT_TEXT;
                value.u.m_szText = EF_ConvertEscapeSequences (ps2, ps - ps2, memBuf);  // convert escape sequences.
                return true;
            }

            ASSERT (*ps == myC('\"'));
            ps++;
        }
        else if (EF_IsNameLeaderChar (ch))  // Is name?
        {
            if (blFound)
                memBuf.Empty ();

            while (true)
            {
                ps2 = ps;
                ps = EF_SkipName (ps);
                
                if (blFound)
                    memBuf.Append (ps2, (ps - ps2) * sizeof (EFChar));

                // Skips spaces.
                while (*ps != myC('\0') && (EFDWord)*ps <= (EFDWord)myC(' '))
                    ps++;

                if (*ps != myC(',') && *ps != myC('.'))  // The ',' or '.' character links names into a whole.
                    break;

                if (blFound)
                    memBuf.AddChar (*ps);
                ps++;

                // Skips spaces.
                while (*ps != myC('\0') && (EFDWord)*ps <= (EFDWord)myC(' '))
                    ps++;

                // ',' or '.' must following a valid name.
                if (EF_IsNameLeaderChar (*ps) == false)
                    return false;
            }

            if (blFound)
            {
                memBuf.AddChar (myC('\0'));
                value.m_enType = EF_EPT_TEXT;  // Saves name as text.
                value.u.m_szText = (EFChar*)memBuf.GetPtr ();
                return true;
            }
        }
        else
            break;  // Invalid character.
    }

    return false;
}

// 调用pObject对象nClassIndex位置处类的默认构造方法。
// nClassIndex提供欲调用其默认构造方法的类在pObject类表中的索引位置
void EF_CallClassDefaultInit (EFObject* pObject, int nClassIndex)
{
    ASSERT (pObject != NULL && nClassIndex >= 0 && nClassIndex < pObject->m_pObjectInfo->m_nClassCount);

    EFSingleObjectRuntimeInfo* pRuntimeInfo = &pObject->m_pObjectInfo->m_aryInfos [nClassIndex];
    while (nClassIndex >= 0)
    {
        EFMethodInfo* pMethodInf = pRuntimeInfo->m_pClassInfo->m_pMethodTable;
        int nCount = pRuntimeInfo->m_pClassInfo->m_nMethodCount;
        for (int j = 0; j < nCount; j++, pMethodInf++)
        {
            if ((pMethodInf->m_dwState & (EF_S_M_INIT | EF_S_M_STATIC)) == EF_S_M_INIT &&
                    pMethodInf->m_nArgCount == 0)
            {
                ASSERT (pMethodInf->m_func != NULL);
                (*(void (*) (EFObject*))pMethodInf->m_func) (pObject);
                return;
            }
        }

        nClassIndex--;
        pRuntimeInfo--;
    }
}

//////////////////////////////////////////////////////////////////////////
// 以下函数主要用于统一Windows和Linux下%s和%S的含义，按Windows下习惯

#if defined (_UNICODE) && defined (_LINUX)

// 将参数中的"%s"替换为"%S", 将"%S"替换为"%s"
EF_STATIC const EFChar* MReviseStringFormater (EFChar* szFormat, EF_CString& format)
{
    ASSERT (szFormat != NULL && format.IsEmpty ());

    format.Empty ();

    EFChar* szReviseFormat = NULL;
	EFChar* pc = szFormat;
	while (*pc != myC('\0'))
	{
		if (*pc++ == myC('%'))
		{
            EFChar ch = *pc;
			if (ch == myC('s') || ch == myC('S'))
            {
                if (szReviseFormat == NULL)
                {
                    format = szFormat;
                    szReviseFormat = format.GetText ();
                    pc = szReviseFormat + (pc - szFormat);
                }
                *pc = ch == myC('s') ? myC('S') : myC('s');
            }
            pc++;
		}
	}

    return (const EFChar*)(szReviseFormat == NULL ? szFormat : szReviseFormat);
}

int _EF_vwprintf_linux (EFChar* szFormat, va_list argList)
{
	EF_CString format;
	return vwprintf (MReviseStringFormater (szFormat, format), argList);
}

int _EF_wprintf_linux (EFChar* szFormat, ...)
{
	va_list argList;
	va_start (argList, szFormat);
	int result = _EF_vwprintf_linux (szFormat, argList);
	va_end (argList);
	return result;
}

int _EF_vswprintf_linux (EFChar* szDest, EFChar* szFormat, va_list argList)
{
	EF_CString format;
	return vswprintf (szDest, (size_t)(0x7FFFFFFF / sizeof (EFChar)), MReviseStringFormater (szFormat, format), argList);
}

int _EF_swprintf_linux (EFChar* szDest, EFChar* szFormat, ...)
{
	va_list argList;
	va_start (argList, szFormat);
	int result = _EF_vswprintf_linux (szDest, szFormat, argList);
	va_end (argList);
	return result;
}

int _EF_vfwprintf_linux(FILE* pFile, EFChar* szFormat, va_list argList)
{
	EF_CString format;
	return vfwprintf(pFile, MReviseStringFormater (szFormat, format), argList);
}

int _EF_fwprintf_linux (FILE* pFile, EFChar* szFormat, ...)
{
	va_list argList;
	va_start (argList, szFormat);
	int result = _EF_vfwprintf_linux (pFile, szFormat, argList);
	va_end (argList);
	return result;
}

int _EF_putws_linux (EFChar* ps)
{
	return wprintf (myT ("%S\r\n"), ps);
}

#endif //_LINUX

//////////////////////////////////////////////////////////////////////////

// MCHCA* 与 UTF-8 之间的转换

char* EF_UCS_to_UTF8 (wchar_t* pUCSText, int nLen, EF_MiniMem& memUTF8Text)
{
    memUTF8Text.Empty ();

	if (nLen == 0)
        return "";

	int nRealLen = wcslen (pUCSText);
	if (nLen < 0 || nLen > nRealLen)
		nLen = nRealLen;

	EFByte* pUTF8 = memUTF8Text.Realloc ((nLen + 1) * 3);  // 一个UCS2字符最多转换到3字节的UTF8编码。

	int byteIndex = 0;
	for(int i = 0; i < nLen; i++)
	{
		EFDWord unicode = pUCSText[i];
		if (unicode == 0)
		{
			break;
		}
		else if (unicode <= 0x7F)
		{
			pUTF8 [byteIndex++] = (EFByte)unicode;
		}
		else if (unicode <= 0x7FF)
		{
			pUTF8 [byteIndex++] = (EFByte)(((unicode & 0x7C0) >> 6) | 0xC0);
			pUTF8 [byteIndex++] = (EFByte)((unicode & 0x3F) | 0x80);
		}
		else
		{
			pUTF8 [byteIndex++] = (EFByte)(((unicode & 0xF000) >> 12) | 0xE0);
			pUTF8 [byteIndex++] = (EFByte)(((unicode & 0xFC0) >> 6) | 0x80);
			pUTF8 [byteIndex++] = (EFByte)((unicode & 0x3F) | 0x80);
		}
	}

	pUTF8 [byteIndex++] = 0;
	memUTF8Text.Realloc (byteIndex);
    return (char*)memUTF8Text.GetPtr ();
}

// 是否是UTF-8后序字节(二进制位是否以10开头)
static bool is_utf8_tail_byte (EFByte b)
{
	return ((b & 0xC0) == 0x80);
}

wchar_t* EF_UTF8_to_UCS (char* pUTF8Text, int nLen, EF_MiniMem& memUCSText)
{
    memUCSText.Empty ();

	if (pUTF8Text == NULL || nLen == 0)
        return L"";

	int nRealLen = strlen((char*)pUTF8Text);
	if(nLen < 0 || nLen > nRealLen)
		nLen = nRealLen;

	wchar_t* pUCSText = (wchar_t*) (memUCSText.Realloc ((nLen + 1) * sizeof(wchar_t)));

	EFByte* pb = (EFByte*)pUTF8Text;
	const EFByte* ps = (EFByte*)pUTF8Text;
	int nIndex = 0;
	EFByte b1 = 0, b2 = 0, b3 = 0;

	while(pb - ps < nLen)
	{
		EFByte b1 = *pb++;
		if ((b1 & 0x80) == 0)
		{ 
			pUCSText[nIndex++] = b1; 
		}
		else if ((b1 & 0xE0) == 0xC0)
		{
			if(pb - ps >= nLen) goto ON_ERROR;
			b2 = *pb++;
			if (is_utf8_tail_byte (b2))
			{
				wchar_t temp = b1 & 0x1F;
				temp = (temp << 6) + (b2 & 0x3F);
				pUCSText[nIndex++] = temp;
			}
			else
				goto ON_ERROR;
		}
		else if ((b1 & 0xF0) == 0xE0)
		{ 
			if(pb - ps >= nLen) goto ON_ERROR;
			b2 = *pb++;
			if (b2 == 0) goto ON_ERROR;
			if(pb - ps >= nLen) goto ON_ERROR;
			b3 = *pb++;

			if (is_utf8_tail_byte (b2) && is_utf8_tail_byte (b3))
			{
				wchar_t temp1 = b1 & 0x0F;
				temp1 = temp1 << 12;
				wchar_t temp2 = b2 & 0x3F;
				temp2 = temp2 << 6;
				pUCSText[nIndex++] = temp1 + temp2 + (b3 & 0x3F);
			}
			else
				goto ON_ERROR;
		}
		else
			goto ON_ERROR;
	}

	pUCSText[nIndex++] = myC('\0');
	memUCSText.Realloc(nIndex * sizeof(wchar_t));
	return (wchar_t*)memUCSText.GetPtr ();

ON_ERROR:
	memUCSText.Free ();
	return L"";
}

// szDataTypeDest缓冲区尺寸至少应该为(EF_TYPE_NAME_MAX_LEN + 1)
void EF_CopyEFTypeName (EFChar* szDataTypeDest, EFChar* szDataTypeSrc)
{
    ASSERT (szDataTypeDest != NULL && szDataTypeSrc != NULL);

	if (szDataTypeSrc != NULL)
    {
        int nCount = EF_TYPE_NAME_MAX_LEN;
        while (nCount > 0 && (*szDataTypeDest++ = *szDataTypeSrc++))
            nCount--;
    }

    *szDataTypeDest = myC('\0');
}

// 判断指定文件名是否为绝对路径文件名
bool EF_IsAbsPathFileName (EFChar* szFileName)
{
    if (szFileName == NULL)
        return false;

#ifdef WIN32
    if (szFileName [0] != myC('\0') && szFileName [1] == myC(':'))
        return true;
#endif

    return (*szFileName == EF_PATH_CHAR);
}

