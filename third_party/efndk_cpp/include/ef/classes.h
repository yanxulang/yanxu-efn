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


//ecl_classes.h

#ifndef __EF_CLASSES_H__
#define __EF_CLASSES_H__

#include <string.h>
#include <stdarg.h>
#include <ef/types.h>

// 默认内存分配时的对齐尺寸。不要太大，否则Detach ()后的指针会浪费很多空间。
#define	_EF_MINIMEM_DEFAULT_MEM_ALIGN_SIZE  32

class EF_MiniMem
{
protected:
	EFByte* m_pData;
	int m_nAllocedSize;
	int m_nSize;

public:
	EF_MiniMem ()
    {
	    m_pData = NULL;
	    m_nAllocedSize = m_nSize = 0;
    }
	virtual ~EF_MiniMem ()
	{
		Free ();
	}

public:
	EFByte* GetPtr () const
	{
		return m_nSize == 0 ? NULL : m_pData;
	}
	int GetSize () const
	{
		return m_nSize;
	}
	EFBool IsEmpty ()
	{
		return m_nSize == 0 ? EF_TRUE : EF_FALSE;
	}
    void Attach (void* pData, int nSize);
	EFByte* Detach ();
    EFByte* Detach (int* pnSize);

public:
	void AddDouble (EFDouble db);
	void AddDWord (EFDWord dw);
	void AddWord (EFWord w);
	void AddInt (EFInt n);
	void AddByte (EFByte bt);
	void AddChar (EFChar ch);
	void AddBool (EFBool bl);
	void AddStr (EFChar* ps);
	void AddText (EFChar* ps);
	void AddBin (EFBinary pbin);
	void AddSpecNumBytes (int nBytesCount, ...);

	int GetDoubleCount ()       {  return m_nSize / sizeof (EFDouble);  }
	int GetDWordCount ()        {  return m_nSize / sizeof (EFDWord);  }
	int GetIntCount ()          {  return m_nSize / sizeof (EFInt);  }
	EFDouble* GetDoublePtr ()    {  return m_nSize == 0 ? NULL : (EFDouble*)m_pData;  }
	EFDWord* GetDWordPtr ()      {  return m_nSize == 0 ? NULL : (EFDWord*)m_pData;  }
	EFInt* GetIntPtr ()          {  return m_nSize == 0 ? NULL : (EFInt*)m_pData;  }

    int ReadFromFile (EFChar* szFileName, int nReadLength = -1);
    bool WriteIntoFile (EFChar* szFileName, int nWriteLength = -1);

#ifdef _LINUX
	bool WriteIntoELFFile (EFChar* szFileName, int nWriteLength = -1);
#endif

    virtual int GetMemAlignSize ()
    {
        return _EF_MINIMEM_DEFAULT_MEM_ALIGN_SIZE;
    }

public:
	EFByte* Alloc (int nSize);
	EFByte* Realloc (int nSize);
	void Free ();

	void xchg (EF_MiniMem& mem);
    void Empty ()
    {
        m_nSize = 0;
    }
	EFBool CopyFrom (EF_MiniMem& mem)
	{
		Empty ();
		return Append (mem.GetPtr (), mem.GetSize ());
	}
    void MoveFrom (EF_MiniMem& mem);
	EFBool CopyFrom (void* pData, int nSize)
	{
		Empty ();
		return Append (pData, nSize);
	}

	EFBool Insert (int nOffset, void* pData, int nSize);
	EFBool InsertInt (int nOffset, EFInt value)
	{
		return Insert (nOffset, &value, sizeof (EFInt));
	}

	EFBool Append (EF_MiniMem& mem);
	EFBool Append (void* pData, int nSize);

	EFBool Replace (int nOffset, int nSize, EFByte* pReplaceData, int nReplaceSize);
	EFBool ReplaceInt (int nOffset, EFInt value)
	{
		return Replace (nOffset, sizeof (EFInt), (EFByte*)&value, sizeof (EFInt));
	}

	EFBool Remove (int nOffset, int nSize);
};

class EF_AlignMem : public EF_MiniMem
{
protected:
    int m_nMemAlignSize;

public:
	EF_AlignMem ()
    {
	    m_nMemAlignSize = _EF_MINIMEM_DEFAULT_MEM_ALIGN_SIZE;
    }
    EF_AlignMem (int nAlignSize)
    {
	    SetMemAlignSize (nAlignSize);
    }
    
    virtual int GetMemAlignSize ()
    {
        return m_nMemAlignSize;
    }
    void SetMemAlignSize (int nAlignSize)
    {
        m_nMemAlignSize = nAlignSize;
        if (m_nMemAlignSize < 0)
            m_nMemAlignSize = 0;
    }
};

#define _EF_DEFINE_C_ARRAY_CLASS(type_name, ary_class_name)                                 \
    class ary_class_name : public EF_AlignMem                                               \
    {                                                                                       \
    public:                                                                                 \
	    ary_class_name ()  { }                                                              \
	    int GetCount ()                                                                     \
	    {                                                                                   \
		    return GetSize () / sizeof (type_name);                                         \
	    }                                                                                   \
	    int GetUpperBound ()                                                                \
	    {                                                                                   \
		    return GetCount () - 1;                                                         \
	    }                                                                                   \
	    void RemoveAll ()                                                                   \
	    {                                                                                   \
		    Free ();                                                                        \
	    }                                                                                   \
        void InitCount (int nElementCount)                                                  \
        {                                                                                   \
            Free ();                                                                        \
            int nSize = nElementCount * sizeof (type_name);                                 \
            EFByte* pb = Alloc (nSize);                                                     \
            if (pb != NULL)                                                                 \
                memset (pb, 0, nSize);                                                      \
        }                                                                                   \
	    type_name* GetData ()                                                               \
	    {                                                                                   \
		    return (type_name*)GetPtr ();                                                   \
	    }                                                                                   \
	    type_name GetAt (int nIndex) const                                                  \
	    {                                                                                   \
		    return ((type_name*)m_pData) [nIndex];                                          \
	    }                                                                                   \
	    type_name& ElementAt (int nIndex)                                                   \
	    {                                                                                   \
		    return ((type_name*)m_pData) [nIndex];                                          \
	    }                                                                                   \
	    type_name operator[] (int nIndex) const                                             \
	    {                                                                                   \
		    return ((type_name*)m_pData) [nIndex];                                          \
	    }                                                                                   \
	    type_name& operator[] (int nIndex)                                                  \
	    {                                                                                   \
		    return ((type_name*)m_pData) [nIndex];                                          \
	    }                                                                                   \
	    void SetAt (int nIndex, type_name element)                                          \
	    {                                                                                   \
		    if (nIndex >= 0 && nIndex < GetCount ())                                        \
			    ((type_name*)m_pData) [nIndex] = element;                                   \
	    }                                                                                   \
	    void Append (const ary_class_name& src)                                             \
	    {                                                                                   \
		    EF_MiniMem::Append (src.GetPtr (), src.GetSize ());                             \
	    }                                                                                   \
	    void Append (const ary_class_name* src)                                             \
	    {                                                                                   \
		    EF_MiniMem::Append (src->GetPtr (), src->GetSize ());                           \
	    }                                                                                   \
	    void Copy (ary_class_name& src)                                                     \
	    {                                                                                   \
		    EF_MiniMem::CopyFrom (src);                                                     \
	    }                                                                                   \
	    void Add (type_name element)                                                        \
	    {                                                                                   \
            EF_MiniMem::Append ((EFByte*)&element, sizeof (type_name));                     \
	    }                                                                                   \
	    void InsertAt (int nIndex, type_name element)                                       \
	    {                                                                                   \
		    EF_MiniMem::Insert (nIndex * sizeof (type_name), (EFByte*)&element, sizeof (type_name));  \
	    }                                                                                   \
	    void RemoveAt (int nIndex, int nCount = 1)                                          \
	    {                                                                                   \
		    EF_MiniMem::Remove (nIndex * sizeof (type_name), sizeof (type_name) * nCount);  \
	    }                                                                                   \
        int Find (type_name data)                                                           \
        {                                                                                   \
            int nCount = GetCount ();                                                       \
            type_name* p = (type_name*)m_pData;                                             \
            for (int i = 0; i < nCount; i++, p++)                                           \
            {                                                                               \
                if (*p == data)                                                             \
                    return i;                                                               \
            }                                                                               \
            return -1;                                                                      \
        }                                                                                   \
    };


_EF_DEFINE_C_ARRAY_CLASS (EFInt, EF_IntArray)
_EF_DEFINE_C_ARRAY_CLASS (EFDWord, EF_DWordArray)
_EF_DEFINE_C_ARRAY_CLASS (void*, EF_PtrArray)

class EF_CString
{
public:
	EF_CString ()  { }
	EF_CString (const EF_CString& stringSrc);
	EF_CString (EFChar* ps);
	EF_CString (EFChar ch);
	EF_CString (EFChar* ps, int nLen)
    {
        SetText (ps, nLen);
    }

public:
    EFDWord GetHash ();
	int GetLength () const;
	EFBool IsEmpty () const;
	void Empty ();
	EFChar* Detach ();
	void xchg (EF_CString& str)
    {
        m_mem.xchg (str.m_mem);
    }

	int myicmp (EFChar* ps);
	int mycmp (EFChar* ps);

	operator EFChar* ()      { return m_mem.IsEmpty () ? EF_EMPTY_SZ_TEXT : (EFChar*)m_mem.GetPtr (); }
	operator const EFChar*() { return m_mem.IsEmpty () ? EF_EMPTY_SZ_TEXT : (const EFChar*)m_mem.GetPtr (); }
	EFChar* GetText ()       { return m_mem.IsEmpty () ? EF_EMPTY_SZ_TEXT : (EFChar*)m_mem.GetPtr (); }

	const EF_CString& operator=(const EF_CString& stringSrc);
	const EF_CString& operator=(EFChar ch);
	const EF_CString& operator=(EFChar* ps);

	const EF_CString& operator+=(const EF_CString& string);
	const EF_CString& operator+=(EFChar ch);
	const EF_CString& operator+=(EFChar* ps);

    void RemoveChar (int nIndex, int nLen = 1);
    void InsertChar (int nIndex, EFChar ch);
    void AddChar (EFChar ch);
    void AddText (EFChar* ps);
    void AddText (EFChar* ps, int nLen);
    void InsertText (int nIndex, EFChar* ps);
    void InsertText (int nIndex, EFChar* ps, int nLen);
	void SetText (EFChar* ps);
	void SetText (EFChar* ps, int nLen);
#ifdef _UNICODE
	void SetText (char* ps, int nLen = -1);
#endif
    void SetDouble (EFDouble db);

	// simple sub-string extraction
	EF_CString Left (int nCount) const;
	EF_CString Right (int nCount) const;

	// upper/lower/reverse conversion
	void MakeUpper ();
	void MakeLower ();

	// trimming whitespace (either side)
	void TrimRight ();
	void TrimLeft ();

	EFBool EndOf (EFChar* ps);
	EFBool IEndOf (EFChar* ps);

    int GetTextPartDataSize () const;

    void Format (EFChar* szFormat, ...);
    void FormatV (EFChar* szFormat, va_list argList);

public:
	EF_MiniMem m_mem;
};

EF_CString operator+(const EF_CString& string1, const EF_CString& string2);
EF_CString operator+(const EF_CString& string, EFChar ch);
EF_CString operator+(const EF_CString& string, EFChar* ps);
EF_CString operator+(EFChar* ps, const EF_CString& string);
EF_CString operator+(EFChar ch, const EF_CString& string);
EFBool operator==(const EF_CString& s1, const EF_CString& s2);
EFBool operator==(const EF_CString& s1, EFChar* s2);
EFBool operator==(EFChar* s1, const EF_CString& s2);
EFBool operator!=(const EF_CString& s1, const EF_CString& s2);
EFBool operator!=(const EF_CString& s1, EFChar* s2);
EFBool operator!=(EFChar* s1, const EF_CString& s2);

//not use?
//#define DEFINE_ETEXT_VAR(text_var_name)  EF_CString text_var_name (sizeof (EFInt))

////////////////////////////////////////////////////////////////////////

class EF_CStringArray : public EF_MiniMem
{
public:
	EF_CStringArray ()  { }
	virtual ~EF_CStringArray ()
	{
		RemoveAll ();
	}

	int GetCount ()
	{
		return GetSize () / sizeof (EFChar*);
	}
	int GetUpperBound ()
	{
		return GetCount () - 1;
	}
	void RemoveAll ();
	EFChar** GetData () const
	{
		return (EFChar**)GetPtr ();
	}

	EFChar* GetAt (int nIndex) const
	{
		EFChar* ps = ((EFChar**)m_pData) [nIndex];
		return ps == NULL ? EF_EMPTY_SZ_TEXT : ps;
	}
	EFChar* operator[] (int nIndex) const
	{
		EFChar* ps = ((EFChar**)m_pData) [nIndex];
		return ps == NULL ? EF_EMPTY_SZ_TEXT : ps;
	}

	void Append (EF_CStringArray& src);
	void Copy (EF_CStringArray& src);
	void RemoveAt (int nIndex, int nCount = 1);

	void SetAt (int nIndex, EFChar* szElement);
	void Add (EFChar* szElement);
    void Add (EFChar* szElement, int nLen);
	void InsertAt (int nIndex, EFChar* szElement);
	void InsertAt (int nIndex, EFChar* szElement, int nLen);

#ifdef _UNICODE
	void SetAt (int nIndex, char* szElement);
	void Add (char* szElement);
	void InsertAt (int nIndex, char* szElement);
#endif

	void SetAt_NoClone (int nIndex, EFChar* szElement);
	void Add_NoClone (EFChar* szElement);
	void InsertAt_NoClone (int nIndex, EFChar* szElement);
	void Append_NoClone (EF_CStringArray& src);  // src will be emptied
	void Append_NoClone (EF_CStringArray* psrc);  // psrc will be emptied
	void Copy_NoClone (EF_CStringArray& src);  // src will be emptied
	void RemoveAt_NotDelete (int nIndex, int nCount = 1);

	int Find (EFChar* ps);
};

#endif //__EF_CLASSES_H__

