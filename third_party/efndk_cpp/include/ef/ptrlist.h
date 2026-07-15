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


//ptrlist.h

#ifndef __EF_PTR_LIST_H__
#define __EF_PTR_LIST_H__

class EF_PtrList
{
private:
	//List的节点
	class Node
	{
	public:
		Node* prev;
		Node* next;
		void* data;
	public:
		Node(void* d = NULL, Node* p = NULL, Node* n = NULL)
			:prev(p), next(n), data(d)
		{}
	};
public:
	//比较节点值是否相等的函数原型
	typedef bool (*LIST_NODE_EQ)(void* data1, void* data2);
	
	class reverse_iterator;
	//迭代器
	class iterator
	{
	private:
		Node* node;
	private:
		//私有构造函数
		iterator(Node* n){node = n;}
	public:
		iterator()
			:node(NULL)
		{}
		iterator& operator = (const iterator& i) 
		{
			if(&i != this)
			{
				node = i.node;
			}
			return *this;
		}
		iterator(const iterator& i)
		{
			*this = i;
		}
		bool operator == (const iterator& i) const
		{
			return (node == i.node);
		}
		bool operator != (const iterator& i)const
		{
			return !(*this == i);
		}
		void*& operator * ()
		{
			if(!node->next) throw "access value of iterator end()";
			return node->data;
		}
		iterator operator ++ (int)
		{
			iterator old = *this;
			++(*this);
			return old;
		}
		iterator operator ++ ()
		{
			if(!node->next) throw "iterator out of range";
			node = node->next;
			return *this;
		}
	friend class reverse_iterator;
	friend class EF_PtrList;
	};

	class reverse_iterator
	{
	private:
		iterator it;
	private:
		reverse_iterator(Node* n):it(n){}
		explicit reverse_iterator(const iterator& i){it = i;}
	public:
		reverse_iterator(){}
		reverse_iterator(const reverse_iterator& i)
		{
			*this = i;
		}
		const reverse_iterator& operator = (const reverse_iterator& i)
		{
			it = i.it;
			return *this;
		}
		bool operator == (const reverse_iterator& i)
		{
			return it == i.it;
		}
		bool operator != (const reverse_iterator& i)
		{
			return it != i.it;
		}
		void*& operator * ()
		{
			if(!it.node->prev) throw "access value of iterator rend()";
			return *it;
		}
		reverse_iterator operator ++ (int)
		{
			reverse_iterator old = *this;
			++(*this);
			return old;
		}
		reverse_iterator operator ++ ()
		{
			if(!it.node->next) throw "iterator out of range";
			it.node = it.node->prev ;
			return *this;
		}
	friend class EF_PtrList;
	};
private:
	//头
	Node head;
	//尾
	Node tail;
	//节点个数
	size_t count;
	//比较函数
	LIST_NODE_EQ fneq;
public:
	//默认比较函数
	static bool pointer_eq(void* data1, void* data2){return data1 == data2;}
	EF_PtrList(LIST_NODE_EQ f = pointer_eq)
		:head(NULL, NULL, &tail), tail(NULL, &head), count(0), fneq(f)
	{}
	virtual ~EF_PtrList(void);
public:
	iterator begin() const {return iterator(head.next);}
	iterator end() const {return iterator((Node*)&tail);}
	reverse_iterator rbegin() const {return reverse_iterator(tail.prev);}
	reverse_iterator rend() const {return reverse_iterator((Node*)&head);}
	void* front() const {return *begin();}
	void* back() const {return *rbegin();}
	size_t size() const {return count;}
	iterator get(size_t index);
	iterator operator [] (size_t index);
public:
	iterator insert(const iterator pos, void* data);
	iterator erase(const iterator pos);
	iterator find(void* data) const;
	iterator push_back(void* data){return insert(end(), data);};
	iterator push_front(void* data){return insert(begin(), data);};
	void pop_back(void){erase(rbegin());}
	void pop_front(void){erase(begin());}
	void clear(void);

	reverse_iterator insert(const reverse_iterator pos, void* data){return reverse_iterator(insert(pos.it, data));}
	reverse_iterator erase(const reverse_iterator pos) {return reverse_iterator(erase(pos.it));}
	reverse_iterator rfind(void* data) const;
};

#endif //__EF_PTR_LIST_H__

