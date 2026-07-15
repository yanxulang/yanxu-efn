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


//EF_PtrList.cpp

#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <ef/ptrlist.h>

EF_PtrList:: ~EF_PtrList(void)
{
	clear();

}
void EF_PtrList::clear(void)
{
	EF_PtrList::iterator it = begin();
	while(it != end())
	{
		Node* pNode = (it++).node;
		delete pNode;
	}

	head.next = &tail;
	tail.prev = &head;
	count = 0;
}
EF_PtrList::iterator EF_PtrList::insert(const EF_PtrList::iterator pos, void* data)
{
	EF_PtrList::Node* node = pos.node;
	EF_PtrList::Node* new_node = new EF_PtrList::Node(data);

	new_node->prev = node->prev;
	new_node->next = node;
	node->prev->next = new_node;
	node->prev = new_node;

	count++;
	return EF_PtrList::iterator(new_node);
}
EF_PtrList::iterator EF_PtrList::erase(const EF_PtrList::iterator pos)
{
	if(count == 0) return end();

	EF_PtrList::Node* node = pos.node;
	EF_PtrList::Node* next = node->next;

	node->prev->next = node->next;
	node->next->prev = node->prev;
	delete node;
	count--;

	return EF_PtrList::iterator(next);
}
EF_PtrList::iterator EF_PtrList::find(void* data) const
{
	EF_PtrList::iterator endIt = this->end();
	for(EF_PtrList::iterator it = begin(); it != endIt; it++)
	{
		if(fneq(data, it.node->data)) return it;
	}
	return endIt;
}
EF_PtrList::reverse_iterator EF_PtrList::rfind(void* data) const
{
	EF_PtrList::reverse_iterator rendIt = this->rend();
	for(reverse_iterator it = rbegin(); it != rendIt; ++it)
	{
		if(fneq(data, *it)) return it;
	}

	return rendIt;
}

EF_PtrList::iterator EF_PtrList::get(size_t index)
{
	if(index >= count)
		return end();

	iterator it = begin();
	for(size_t i = 0; i < index; ++i)
		it++;
	return it;
}

EF_PtrList::iterator EF_PtrList::operator [] (size_t index)
{
	return get(index);
}

