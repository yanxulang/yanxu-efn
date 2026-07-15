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


//ecl_functions.h

#ifndef __EF_FUNCTIONS_H__
#define __EF_FUNCTIONS_H__

#include <ef/types.h>
#include <ef/classes.h>

EFBool EF_IsEmptyStr (EFChar* ps);
EFBool EF_IsEmptyText (EFText etext);
EFBool EF_IsEmptyBin (EFBinary bin);
EFBool EF_IsEmptyArray (EFArray array);

void EF_CopyEFTypeName (EFChar* szDataTypeDest, EFChar* szDataTypeSrc);

int EF_atoi (EFChar* ps);
double EF_atof (EFChar* ps);
int EF_strlen (EFChar* ps);
int EF_strlen (EFText etext);
void EF_strcpy (EFChar* szDest, EFChar* szSource);
void EF_strncpy (EFChar* szDest, EFChar* szSource, int nCount);
void EF_strcat (EFChar* szDest, EFChar* szSource);
void EF_strncat (EFChar* szDest, EFChar* szSource, int nCount);
int EF_stricmp (EFChar* szSource, EFChar* szDest);
int EF_strcmp (EFChar* szSource, EFChar* szDest);
int EF_strncmp (EFChar* ps1, EFChar* ps2, int count);
int EF_strnicmp (EFChar* ps1, EFChar* ps2, int count);
EFChar* EF_tolower(EFChar* pText);
EFChar* EF_toupper(EFChar* pText);
EFChar EF_tolowerc(EFChar c);
EFChar EF_toupperc(EFChar c);
EFChar* EF_strchr (EFChar* ps, EFChar ch);
EFChar* EF_strrchr (EFChar* ps, EFChar ch);
EFChar* EF_strstr (EFChar* ps1, EFChar* ps2);
EFChar* EF_strtok (EFChar * ps, EFChar * szControl);
int EF_max (int n1, int n2);
int EF_min (int n1, int n2);

char* EF_GetMBSStr (EFChar* ps, EF_MiniMem& memBuf);

EFText EF_CloneEFText (EFText etext); //EF str -> EF str
EFText EF_CloneEFText (EFChar* ps); //C str -> EF str;
EFText EF_CloneEFText (EFChar* ps, int nLen);
EFChar* EF_CloneEFTextToStr (EFText etext);
EFChar* EF_CloneStr (EFChar* text); //C str -> C str
EFChar* EF_CloneStr (EFChar* text, EFInt len);
#ifdef _UNICODE
	EFChar* EF_CloneStr (char* ps);
    EFText EF_CloneEFText (char* ps);
#endif

int EF_GetEFBinLength (EFBinary bin);
EFByte* EF_GetEFBinData (EFBinary bin);
EFBinary EF_CloneEFBin (EFBinary bin);
EFBinary EF_CloneEFBin (void* pData, EFInt nDataLength);

EFArray EF_CloneEFArray (EFArray pAryInf, EF_DatatypeType dtType);

int EF_GetEFArraySize (EFArray array, void** ppAryData = NULL);
void* EF_GetEFArrayData (EFArray array, int* pnArraySize = NULL);
int EF_GetEFArrayDim(EFArray array); //取维数

struct EFObject;
void EF_CloneIntoEFTextVar (EFText* ppTextVar, EFText new_text);
void EF_CloneIntoEFBinVar (EFBinary* ppBinVar, EFBinary new_bin);
void EF_CloneIntoEFObjectVar (EFObject** ppObjectVar, EFObject* new_object);
void EF_CloneIntoEFArrayVar (EFArray* ppArrayVar, EFArray new_array);

void* EF_GetAddressByIntIndex(void* pData, int index);
EFInt EF_GetIntByIndex(void* pData, int index);
void EF_SetIntByIndex(void* pData, int index, EFInt value);
void* EF_GetEFBinDataAndLength (EFBinary pBin, EFInt* pLength);
void EF_FreeEFVar (void* pVarAdr, EFChar* szVarDataType);
EFChar* EF_GetPureDatatype (EFChar* szDataType, EFBool* pblIsAry = NULL);

EFDWord EF_GetStrHash (EFChar* k);
EFDWord EF_GetStrHash (EFChar* k, int nLen);
EFDWord EF_GetFriendlyNameHash (EFChar* k);
EFDWord EF_GetFriendlyNameHash (EFChar* k, int nLen);

bool EF_IsSameStr (EFChar* szText1, EFDWord dwHashValue1, EFChar* szText2, EFDWord dwHashValue2);
bool EF_IsSameFriendlyName (EFChar* szFriendName1, EFDWord dwFriendNameHashValue1,
        EFChar* szFriendName2, EFDWord dwFriendNameHashValue2, bool blIsMethodFriendName);
bool EF_IsSameFriendlyName (EFChar* szFriendName1, EFChar* szFriendName2, bool blIsMethodFriendName);

EF_DatatypeType EF_GetDatatypeType (EFChar* szDataType, EFDWord dwDataTypeHash);
int EF_GetDatatypeSize (EFChar* szDataType, EFDWord dwDataTypeHash);
int EF_GetDatatypeSize (EF_DatatypeType dtType);
int EF_GetDatatypeSizeOfArrayElement (EF_DatatypeType dtType);
EFChar* EF_GetDatatypeTypeDisplayName (EF_DatatypeType enType);
//!!! 注意，由于返回的是EF_CString对象，因此如果要将返回值传递到类似printf类的函数，
// 必须调用所返回EF_CString对象的GetText方法取得指针传递过去，否则会出错。
EF_CString EF_GetDatatypeDisplayNameString (EFChar* szDataType, EFDWord dwDataTypeHash);

void* EF_malloc (int nReqMemSize);
void* EF_realloc (void* pPtr, int nReallocMemSize);
void EF_free (void* pPtr);

bool EF_IsEFConstData (void* pPtr);

void EF_GetEFLibSearchPaths(EF_CStringArray& paths);
EFDWord EF_GetDataSum (EFByte* pb, int nDataSize);

#include <eflib/sys/runtime_err.h>
void EF_ReportEFRuntimeError (EFChar* szClsLibName, EFInt nErrorCode, EFChar* szErrorText);

void* EF_GetEFObjectDataByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex);
void EF_SetEFObjectDataByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex, void* value);

#define EF_GetIntFieldOfEFObject(pObject,nClassIndex,nItemIndex)  \
            (EFInt)(EF_GetEFObjectDataByIntIndex (pObject, nClassIndex, nItemIndex))
#define EF_SetIntFieldOfEFObject(pObject,nClassIndex,nItemIndex,nValue)  \
            EF_SetEFObjectDataByIntIndex (pObject, nClassIndex, nItemIndex, (void*)nValue)

EFDouble EF_GetDoubleFieldOfEFObjectByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex);
void EF_SetDoubleFieldOfEFObjectByIntIndex (EFObject* pObject, int nClassIndex, int nItemIndex, EFDouble dblValue);

EF_CString EF_GetEFEventShortName (EFChar* szEventClassName);
EF_CString EF_GetEFEventShortName (EFObject* pEventObject);
EFChar* EF_GetClassNameOfEFObject (EFObject* pObject);

bool EF_IsNameLeaderChar (EFChar ch);
bool EF_IsNameNextChar (EFChar ch);
EFChar* EF_SkipName (EFChar* ps);
bool EF_IsValidName (EFChar* szName, bool blAllowInnerMidChar = false, bool blAllowNameHideChar = false);
bool EF_IsValidFriendlyName (EFChar* szFriendName, int nMethodArgCount);
bool EF_IsValidTypeName (EFChar* szName);
bool EF_IsValidDataTypeName (EFChar* szDataTypeName, bool blAllowCommaCompart = false);
EFChar* EF_GetFileNamePart (EFChar* szPath);
bool EF_GetPathPart (EFChar* szFullFileName, EF_CString& strPath);
bool EF_IsAbsPathFileName (EFChar* szFileName);
void EF_SplitStrings (EFChar* ps, EF_CStringArray& strary);
bool EF_IsCmbDatatype (EFChar* szDataType);
void EF_CallClassDefaultInit (EFObject* pObject, int nClassIndex);
EFChar* EF_ConvertEscapeSequences (EFChar* szEFormatText, int nLen, EF_MiniMem& memBuf);

char* EF_UCS_to_UTF8 (wchar_t* pUCSText, EFInt nLen, EF_MiniMem& memUTF8Text);
wchar_t* EF_UTF8_to_UCS (char* pUTF8Text, EFInt nLen, EF_MiniMem& memUCSText);


// 定义一个可用于存储C/C++指针的EF数据成员（整数型，隐藏，无名称）
#define EF_FIELD_OF_C_POINTER \
	EF_FIELD (myT(""), myT(""), EF_S_HIDDEN, EF_INT)

//////////////////////////////////////////////////////////////////////////

typedef enum
{
    EF_EPT_NULL = 0,
    EF_EPT_NUM  = 1,
    EF_EPT_BOOL = 2,
    EF_EPT_TEXT = 3
}
EF_ExtendedPropertyType;

struct EF_ExtendedPropertyValue
{
    EF_ExtendedPropertyType m_enType;

    union
    {
        EFDouble m_double; // It's invalid if m_enType equals EF_EPT_NUM.
        EFBool m_bool;     // EF_EPT_BOOL
        EFChar* m_szText;  // EF_EPT_TEXT
    } u;
};
typedef struct EF_ExtendedPropertyValue EF_ExtendedPropertyValue;

// Returns true if specified property has found, not found or failed returns false.
bool EF_GetExtendedPropertyValue (EFChar* szExtendPropertiesAggregate, EFChar* szFindPropertyName,
        EF_ExtendedPropertyValue& value, EF_MiniMem& memBuf);

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#ifdef _LINUX
	    int _EF_vwprintf_linux(EFChar* szFormat, va_list argList);
	    int _EF_wprintf_linux (EFChar* szFormat, ...);
	    int _EF_vswprintf_linux(EFChar* szDest, EFChar* szFormat, va_list argList);
	    int _EF_swprintf_linux (EFChar* szDest, EFChar* szFormat, ...);
	    int _EF_vfwprintf_linux(FILE* pFile, EFChar* szFormat, va_list argList);
	    int _EF_fwprintf_linux (FILE* pFile, EFChar* szFormat, ...);
	    int _EF_putws_linux(EFChar* ps);

		#define EF_printf    _EF_wprintf_linux
		#define EF_vprintf   _EF_vwprintf_linux
		#define EF_puts      _EF_putws_linux
		#define EF_sprintf   _EF_swprintf_linux
		#define EF_vsprintf  _EF_vswprintf_linux
		#define EF_fprintf   _EF_fwprintf_linux
		#define EF_vfprintf  _EF_vfwprintf_linux
		#define EF_getchar   getwchar
	#else
		#define EF_printf    wprintf
		#define EF_vprintf   vwprintf
		#define EF_puts      _putws
		#define EF_sprintf   swprintf
		#define EF_vsprintf  vswprintf
		#define EF_fprintf   fwprintf
		#define EF_vfprintf  vfwprintf
		#define EF_getchar   getwchar
	#endif
#else
    #define EF_printf    printf
    #define EF_vprintf   vprintf
    #define EF_puts      puts
    #define EF_sprintf   sprintf
    #define EF_fprintf   fprintf
    #define EF_vsprintf  vsprintf
    #define EF_vfprintf  vfprintf
    #define EF_getchar   getchar
#endif


#endif //__EF_FUNCTIONS_H__
