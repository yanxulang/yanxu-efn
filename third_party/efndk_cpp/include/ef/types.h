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


#ifndef __ETYPES_H__
#define __ETYPES_H__

/*
对应于每种数据类型变量内的数据格式：

如果不为数组：
	EF_INT:           数据类型为EFInt
    EF_DOUBLE:        数据类型为EFDouble
    EF_BOOL:          数据类型为EFBool, EF_TRUE为真, EF_FALSE为假.
	EF_TEXT:          EFText数据指针。EFText数据格式为：
                          1、一个EFInt记录被引用计数和类型。
		                  2、一个EFInt记录文本长度（总字符个数, 不含结束0）
						  3、后跟以0结束的unicode字符串数据
	EF_BIN:           EFBinary数据指针，EFBinary数据格式为：
                          1、一个EFInt记录被引用计数和类型。
		                  2、一个恒定为数值1的EFInt
		                  3、一个EFInt记录数据的长度
		                  4、相应长度的数据
	对象:               EFObject*指针，如果为NULL表示空对象。
    接口:               EFInterface*指针，如果为NULL表示空接口。

如果为数组（引用数组元素时索引从0开始）：
    EFArray数据指针，EFArray数据格式为：
    1、一个EFInt记录被引用计数和类型。
	2、一个EFInt记录该数组的维数（必定大于0）
	3、对应数目的EFInt值记录对应维的成员数目，从高维到低维顺序记录，如 a[3][2]，则记录顺序为 3, 2 。
	4、数组数据格式为：
		EF_TEXT、EF_BIN、对象、接口数据:
			为相应的数据指针数组，每个指针指向一个对应数据块
		其它数据类型:
			为不为数组情况下数据的顺序排列.

附，数据指针为NULL的三种情况：
    1、对象中的对象或接口类型成员可能为NULL；
    2、对象或接口类型数组的成员可能为NULL；
    3、当方法的类/接口/数组型参数定义了EF_S_ARG_NULLABLE标志，则该方法的此参数可能会接收到NULL对象或接口值。
*/


//////////////////////////////////////////////////////////////////////////

// 以下类型分别对应EF中的五种基本类型（整数，小数，逻辑，文本，字节集）
typedef int              EFInt;
typedef double           EFDouble;
typedef int              EFBool;
typedef unsigned int*    EFText;
typedef unsigned char*   EFBinary;

// 对应EF中的数组类型
typedef EFInt*           EFArray;

// EFBool逻辑值数据，真和假必须使用这两个数值。
#define EF_TRUE    1   // 必须为1，不可更改
#define EF_FALSE   0   // 必须为0，不可更改


// 以下类型不与任何EF类型存在对应关系，供本地库内部使用
typedef unsigned char    EFByte;
typedef unsigned short   EFWord;
#include <stdint.h>
typedef uintptr_t        EFDWord;


// 通用函数指针，其变量用作记录函数地址
typedef void (*EF_PFUNC) ();

//////////////////////////////////////////////////////////////////////////

#ifdef UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif
#ifdef _UNICODE
	#ifndef UNICODE
		#define UNICODE
	#endif
#endif


#ifdef _UNICODE
    #include <wchar.h>
 	#define EFChar wchar_t
    #define myT(x)   L ## x
    #define myC(x)   L ## x
#else
    #define EFChar char
    #define myT(x)  x
    #define myC(x)  x
#endif

////////////////////////////////////////////////////////////////////////////////
// 用作处理数据块的类型和被引用计数

#define EF_DATA_TYPE_SHIFT_NUM  (sizeof (EFInt) * 8 - 3)
#define EF_DATA_TYPE_MASK       (7 << EF_DATA_TYPE_SHIFT_NUM)
#define EF_REF_COUNT_MASK       (~EF_DATA_TYPE_MASK)

// 受GC管理的数据块类型 (GCDATA-TYPE)
#define EF_GCDT_PUREDATA           (0 << EF_DATA_TYPE_SHIFT_NUM)  // 单纯的数据块, 其中没有参考到其它数据块的成员.
#define EF_GCDT_TEXT_OR_BIN_ARRAY  (1 << EF_DATA_TYPE_SHIFT_NUM)  // 文本或字节集数组数据
#define EF_GCDT_OBJECT             (2 << EF_DATA_TYPE_SHIFT_NUM)  // 对象数据
#define EF_GCDT_OBJECT_ARRAY       (3 << EF_DATA_TYPE_SHIFT_NUM)  // 对象数组数据
#define EF_GCDT_INTERFACE          (4 << EF_DATA_TYPE_SHIFT_NUM)  // 接口数据
#define EF_GCDT_INTERFACE_ARRAY    (5 << EF_DATA_TYPE_SHIFT_NUM)  // 接口数组数据

#define EF_GET_TRUE_REF_COUNT(nRefCountData)  ((EFInt)(nRefCountData) & EF_REF_COUNT_MASK)
#define EF_SET_TRUE_REF_COUNT(pRefCountData,nTrueRefCount)  (*(EFInt*)(pRefCountData) |= nTrueRefCount)
#define EF_GET_GCDATA_TYPE(nRefCountData)       ((EFInt)(nRefCountData) & EF_DATA_TYPE_MASK)

////////////////////////////////////////////////////////////////////////////////
// 下面信息用作记录和处理常量数据

struct EFConstDataHeader;
struct EFConstDataHeader
{
    int m_nConstDataSize;
    EFConstDataHeader* m_prev;  // 指向前一个常量区
    EFConstDataHeader* m_next;  // 指向后一个常量区
    void* m_pLibInfo;  // 记录本常量区所处的类库，实际类型为EFLibInfo*
};
typedef struct EFConstDataHeader EFConstDataHeader;

typedef struct tagEFEmptyConstData
{
    EFConstDataHeader m_header;

    EFChar m_chEmptyText;
    EFInt m_empty_etext [3];
    EFInt m_empty_earray [3];
}
EFEmptyConstData;

#define EF_DEFINE_EMPTY_CONST_DATA   \
    EFEmptyConstData g_EmptyConst =                        \
    {                                                      \
        {  sizeof (EFEmptyConstData), NULL, NULL, NULL  }, \
        myC('\0'),                                         \
        {  (EF_GCDT_PUREDATA | 0), 0, 0  },                 \
        {  (EF_GCDT_PUREDATA | 0), 1, 0  },                 \
    };

extern EFEmptyConstData g_EmptyConst;

//!!! 空字节集(EFBinary)、空数组(EFArray)、空文本(EFText, EFChar*, char*)等数据请务必使用下面对应宏来表示，以免EF_free释放内存出错。
#define EF_EMPTY_TEXT          ((EFText)g_EmptyConst.m_empty_etext)
#define EF_EMPTY_BIN           ((EFBinary)g_EmptyConst.m_empty_earray)
#define EF_EMPTY_ARRAY         ((EFArray)g_EmptyConst.m_empty_earray)
#define EF_EMPTY_SZ_ANSI_TEXT  ((char*)&g_EmptyConst.m_chEmptyText)
#define EF_EMPTY_SZ_TEXT       (&g_EmptyConst.m_chEmptyText)

////////////////////////////////////////////////////////////////////////////////

// 返回指定EFText格式文本数据中的文本内容(EFChar*)、文本长度(EFInt)
#define EF_GET_TEXT(etext_data)      (EFChar*)((EFByte*)etext_data + sizeof (EFInt) + sizeof (EFInt))
#define EF_GET_TEXT_LEN(etext_data)  *(EFInt*)((EFByte*)etext_data + sizeof (EFInt))

////////////////////////////////////////////////////////////////////////////////

// 所有基本系统数据类型必须以EF_BASE_DATA_TYPE_LEADER_CHAR开头，并且为其专用。
#define EF_BASE_DATA_TYPE_LEADER_CHAR  myC('_')

// 所有数组数据类型必须以EF_ARRAY_DATA_TYPE_LEADER_CHAR开头，并且为其专用。
#define EF_ARRAY_DATA_TYPE_LEADER_CHAR  myC('@')
#define EF_ARRAY_DATA_TYPE_LEADER_TEXT  myT ("@")

// 数据类型中间分隔符号
#define EF_TYPE_DELIMITER_CHAR  myC('.')
#define EF_TYPE_DELIMITER_TEXT  myT (".")

// 内嵌类型/方法名中间分隔符号
#define EF_INNER_NAME_DELIMITER_CHAR  myC('/')
#define EF_INNER_NAME_DELIMITER_TEXT  myT ("/")

// 名称隐藏符
// 用作建立一些无法由源代码输入但却能正常使用的名称，可以放在名称的任何位置。
#define EF_HIDDEN_NAME_DELIMITER_CHAR  myC('@')
#define EF_HIDDEN_NAME_DELIMITER_TEXT  myT ("@")

#define EF_IS_ARRAY_DATATYPE_SZ(szDataType)  \
        (*szDataType == EF_ARRAY_DATA_TYPE_LEADER_CHAR)

#define EF_IS_BASE_DATATYPE_SZ(szDataType)  \
    (*szDataType == EF_BASE_DATA_TYPE_LEADER_CHAR || \
        EF_IS_ARRAY_DATATYPE_SZ(szDataType) && *(szDataType + 1) == EF_BASE_DATA_TYPE_LEADER_CHAR)

#define EF_TYPE_NAME_MAX_LEN  (256 - 1)  // 数据类型全名的最大长度。

//////////////////////////////////////////////////////////////////////////
// 名称定义

// 基本类型标识，用作指定成员或参数的数据类型
#define	EF_INT         myT ("_i")
#define	EF_DOUBLE      myT ("_db")
#define	EF_BOOL        myT ("_b")
#define	EF_TEXT        myT ("_s")
#define	EF_BIN         myT ("_bn")

// 基本类型的中文显示名称
#define	EF_NAME_INT          myT ("整数")
#define	EF_NAME_DOUBLE       myT ("小数")
#define	EF_NAME_BOOL         myT ("逻辑")
#define	EF_NAME_TEXT         myT ("文本")
#define	EF_NAME_BIN          myT ("字节集")

// 基本类型的英文文显示名称
#define	EF_NAME_INT_E        myT ("int")
#define	EF_NAME_DOUBLE_E     myT ("double")
#define	EF_NAME_BOOL_E       myT ("bool")
#define	EF_NAME_TEXT_E       myT ("string")
#define	EF_NAME_BIN_E        myT ("binary")

// 空类型标识，用作指定某方法没有返回值
#define	EF_NULL            myT ("")  //!!! 必须为空文本，否则将影响程序其它地方。

// 空类型的中英文显示名称
#define	EF_NAME_NULL         myT ("空")
#define	EF_NAME_NULL_E       myT ("null")

//////////////////////////////////////////

#define EF_NAME_INIT            myT ("初始化")      // EF类初始化方法(构造函数)的固定名称
#define EF_NAME_CLEAN           myT ("清理")        // EF类清理方法(析构函数)的固定名称
#define EF_NAME_STATIC_INIT     myT ("静态初始化")  // EF类静态初始化方法的固定名称
#define EF_NAME_MAIN_METHOD     myT ("启动")        // EF类启动方法的固定名称，该方法的固定格式为([]内的可以省略)：
                                                    //   静态 [整数] 启动 ([文本数组 命令行参数])
#define EF_NAME_INIT_E          myT ("init")
#define EF_NAME_CLEAN_E         myT ("clean")
#define EF_NAME_STATIC_INIT_E   myT ("StaticInit")
#define EF_NAME_MAIN_METHOD_E   myT ("main")

#define EF_NAME_TRUE         myT ("真")
#define EF_NAME_FALSE        myT ("假")
#define EF_NAME_TRUE_E       myT ("true")
#define EF_NAME_FALSE_E      myT ("false")

// 在内嵌类型中指向其所处类型对象实例的成员名称。
#define EF_NAME_PARENT_OBJECT_FIELD    myT ("父对象")
#define EF_NAME_PARENT_OBJECT_FIELD_E  myT ("parent")

////////////////////////////////////////////////////////////////////////////////

// 如果有任何改动，应该同步更改下面的判断宏命令。
typedef enum
{
    EF_DT_NULL = 0,

    EF_DT_INT,
    EF_DT_DOUBLE,
    EF_DT_BOOL,
    EF_DT_TEXT,
    EF_DT_BIN,
    EF_DT_CLASS_INTERFACE_ENUM,  // 数据类型为类/接口/枚举

    EF_DT_INT_ARRAY,
    EF_DT_DOUBLE_ARRAY,
    EF_DT_BOOL_ARRAY,
    EF_DT_TEXT_ARRAY,
    EF_DT_BIN_ARRAY,
    EF_DT_CLASS_INTERFACE_ENUM_ARRAY,  // 数据类型为类/接口/枚举数组
}
EF_DatatypeType;

// 判断该数据类型的数据是否需要释放
// （注意当本宏返回真但datatype_type所代表的数据类型为枚举时不需要释放，因此使用本宏必须首先处理好此特例）
#define EF_IS_NEED_FREE_DATATYPE(datatype_type)  \
        (datatype_type >= EF_DT_TEXT)

// 判断该数据类型数组的成员数据是否需要释放（注意使用前必须确定datatype_type所代表的数据类型不为枚举数组）
#define EF_IS_NEED_FREE_DATATYPE_ARRAY_ELEMENT(datatype_type)  \
        (datatype_type >= EF_DT_TEXT_ARRAY)

#define EF_IS_EXTENDED_DATATYPE_NA(datatype_type)  \
        (datatype_type == EF_DT_CLASS_INTERFACE_ENUM)

#define EF_IS_EXTENDED_DATATYPE_ARRAY(datatype_type)  \
        (datatype_type == EF_DT_CLASS_INTERFACE_ENUM_ARRAY)

// 不包括数组类型
#define EF_IS_BASE_DATATYPE_NA(datatype_type)  \
        (datatype_type >= EF_DT_INT && datatype_type <= EF_DT_BIN)

// 不包括数组类型
#define EF_IS_NUMBER_DATATYPE_NA(datatype_type)  \
        (datatype_type == EF_DT_INT || datatype_type == EF_DT_DOUBLE)

// 判断是否为小数数据
#define EF_IS_DOUBLE_DATATYPE_NA(datatype_type)  \
        (datatype_type == EF_DT_DOUBLE)

#define EF_IS_NUMBER_DATATYPE_ARRAY(datatype_type)  \
        (datatype_type == EF_DT_INT_ARRAY || datatype_type == EF_DT_DOUBLE_ARRAY)

#define EF_IS_ARRAY_DATATYPE(datatype_type)  \
        (datatype_type >= EF_DT_INT_ARRAY)

#define EF_IS_BASE_DATATYPE(datatype_type)  \
        (EF_IS_BASE_DATATYPE_NA (datatype_type) || datatype_type >= EF_DT_INT_ARRAY && datatype_type <= EF_DT_BIN_ARRAY)

////////////////////////////////////////////////////////////////////////////////

#define EF_STATIC  static

#ifndef NULL
    #define NULL    0
#endif

#ifdef WIN32
    #define EF_PATH_CHAR       myC('\\')
    #define EF_PATH_CHAR_TEXT  myT ("\\")
#else
    #define EF_PATH_CHAR       myC('/')
    #define EF_PATH_CHAR_TEXT  myT ("/")
#endif

#ifdef _DEBUG
    #include <assert.h>
    #define ASSERT(t)       assert (t)
    #define FAIL            assert (0)
    #define DEFAULT_FAIL    default: FAIL; break;
#else
    #define ASSERT(t)
    #define FAIL
    #define DEFAULT_FAIL
#endif

/* Deprecated functions such as wcscpy, sprintf, localtime */
#ifdef __VISUALC__
#if __VISUALC__ >= 1400
	#ifndef _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE      // 阻止类似 warning C4996: 'wcscpy' was declared deprecated
	#endif

	#ifndef _CRT_NON_CONFORMING_SWPRINTFS
		#define _CRT_NON_CONFORMING_SWPRINTFS // 阻止类似 warning C4996: 'swprintf' was declared deprecated
	#endif
#endif
#endif

#endif //__ETYPES_H__
