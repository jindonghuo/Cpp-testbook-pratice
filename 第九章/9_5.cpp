#ifndef LINKLIST_H
#define LINKLIST_H
#include"Node.h"
#include<cassert>

template<class T>
class LinkList{
private:
	//数据成员                    https://blog.csdn.net/lslfox/article/details/106753635
	Node<T> *head, *rear;	// 表头，表尾
	Node<T> *prePtr, *curPtr;	// 记录表当前遍历位置的指针，由插入和删除操作更新
	int size;	// 表中的元素个数
	int position; //当前元素在表中的位置序号，由函数reset使用。
	//函数成员
	//生成新结点，数据域为item，指针域为ptrNext。
	Node<T> * newNode(const T &item, Node<T> *ptrNext = NULL);
	// 释放结点
	void freeNode(Node<T> *p);
	//将链表L复制到当前表（假设当前表为空）
	//被复制构造函数和“operator”调用
	void copy(const LinkList<T> & L);
public:
	LinkList();		//构造函数
	LinkList(const LinkList<T> &L);	//复制构造函数
	~LinkList();	//析构函数
	LinkList<T>& operator=(const LinkList<T> &L);	//重载赋值运算符

	int getSize() const;	//返回链表中元素个数
	bool isEmpty() const;	//链表是否为空

	void reset(int pos = 0);	//初始化游标位置
	void next();				//是游标移动到下一个结点
	bool endOfList() const;		//游标是否到了链尾
	int currentPosition() const;	//返回游标当前位置

	void insertFront(const T &item);	//在表头插入结点
	void insertRear(const T &item);		//在表尾插入结点

	void insertAt(const T &item);		//在当前结点之前插入结点
	void insertAfter(const T &item);	//在当前结点之后插入结点

	T deleteFront();		//删除头结点
	void deleteCurrent();	//删除当前结点

	T& data();				//返回对当前结点成员数据的引用
	const T& data() const;	//返回对当前结点成员数据的常引用

	//清空链表：释放所有结点的内存空间。被析构函数和“operator=”调用
	void clear();
};

template<class T>
void LinkList<T>::freeNode(Node<T> *p){
	delete p;
}

template<class T>
void LinkList<T>::copy(const LinkList<T> &L){
	head = L.head;
	rear = L.head;
	curPtr = L.curPtr;
	prePtr = L.prePtr;
	size = L.size;
	position = L.position;
}

template<class T>
LinkList<T>::LinkList():head(NULL), rear(NULL), prePtr(NULL), curPtr(NULL), size(0), position(0)	{}

template<class T>
LinkList<T>::LinkList(const LinkList<T> &L){
	copy(L);
}

template<class T>
LinkList<T>::~LinkList(){
	clear();
	delete prePtr;
	delete curPtr;
}

template<class T>
LinkList<T> &LinkList<T>::operator=(const LinkList<T> &L){
	copy(L);
}

template<class T>
int LinkList<T>::getSize() const{
	return size;
}

template<class T>
bool LinkList<T>::isEmpty() const{
	return size == 0;
}

template<class T>
void LinkList<T>::reset(int pos){
	if (pos >= 0 && pos <= size){
		position = 0;
		curPtr = head;
		prePtr = NULL;
		while (pos--)	next();
	}
	else {
		position = pos;
		curPtr = prePtr = NULL;
	}
}

template<class T>
void LinkList<T>::next(){
	++position;
	prePtr = curPtr;
	if (curPtr != NULL)	curPtr = curPtr->nextNode();
}

template<class T>
bool LinkList<T>::endOfList() const{
	return position == size - 1;
}

template<class T>
int LinkList<T>::currentPosition() const{
	return position;
}

template<class T>
Node<T> *LinkList<T>::newNode(const T &item, Node<T> *ptrNext){
	Node<T> *p = new Node<T>(item, ptrNext);
	assert(p != NULL);
	return p;
}

template<class T>
void LinkList<T>::insertFront(const T &item){
	head = newNode(item, head);
	if(isEmpty())
		rear = head;
	++size;
	reset(++position);
}

template<class T>
void LinkList<T>::insertRear(const T &item){
	Node<T> *p = newNode(item);
	if (isEmpty()) {
		head = rear = p;
	} else {
		rear->insertAfter(p);
		rear = p;
	}
	++size;
	reset(position);
}

template<class T>
void LinkList<T>::insertAt(const T &item){
	if(curPtr != NULL){
		Node<T> *p = newNode(item, curPtr);
		if (prePtr != NULL)	prePtr->insertAfter(p);
		else head = rear = p;
		++size;
		reset(++position);
	}
}

template<class T>
void LinkList<T>::insertAfter(const T &item){
	if(curPtr != NULL){
		Node<T> *p = newNode(item, curPtr->next);
		curPtr->insertAfter(p);
		++size;
		reset(position);
	}
}

template<class T>
T LinkList<T>::deleteFront(){
	assert(!isEmpty());
	Node<T> *p = head;
	head = head->nextNode();
	if (--size == 0)	head = rear = NULL;
	reset(--position);
	T item = p->data;
	freeNode(p);
	return item;
}

template<class T>
void LinkList<T>::deleteCurrent(){
	assert(!isEmpty());
	Node<T> *p = curPtr;
	if(curPtr){
		if (curPtr == head)	head = head->nextNode();
		else if (curPtr == rear)	rear = prePtr;
		else if (prePtr != NULL)	prePtr->deleteAfter();
		freeNode(curPtr);
		--size;
		reset(position);
	}
}

template<class T>
T &LinkList<T>::data(){
	return curPtr->data;
}

template<class T>
const T& LinkList<T>::data() const{
	return curPtr->data;
}

template<class T>
void LinkList<T>::clear(){
	while (!isEmpty())
		deleteFront();
}

#endif
