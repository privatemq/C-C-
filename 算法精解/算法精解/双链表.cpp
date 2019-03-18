#include<stdlib.h>
#include<string.h>
typedef struct DListElmt    //节点属性
{
	void *data;
	struct DListElmt *prev;
	struct DListElmt *next;
}DListElmt;

typedef struct DList
{
	int size;
	int(*match)(const void *key1, const void *key2);
	void(*destroy)(void *data);
	DListElmt *head;
	DListElmt *tail;
}DList;

#define dlist_is_head(element) ((element)->perv == NULL ? 1:0)  //是否是头
#define dlist_is_tail(element) ((element)->next == NULL ? 1:0)  //是否是尾

//创建链表
void dlist_init(DList *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

//删除节点
int dlist_remove(DList *list, DListElmt *element, void **data)
{
	if (element == NULL || list->size == 0)
		return 1;
	*data = element->data;

	if (element == list->head)       //如果是头节点，则判空。
	{
		if (list->head == NULL)    //为空则是最后一个节点，清空连接
			list->tail = NULL;
		else
			element->next->prev = NULL;  //非空，则摘除节点，准备删除
	}
	else   //如果不是头结点，摘除。删除
	{
		element->prev->next = element->next; //删除节点的前一个指向删除节点的后一个
		if (element->next == NULL)    //判断是否为最后一个节点
			list->tail = element->prev;
		else
			element->next->prev = element->prev; //删除节点的后一个指向删除节点的前一个
	}

	free(element);
	list->size--;
	return 0;
}

//删除链表
void dlist_destroy(DList *list)
{
	void *data;
	while (list->size > 0)
	{
		if (dlist_remove(list, list->tail, (void **)&data) == 0
			&& list->destroy != NULL)
			//如果节点删除成功，则删除节点对应存储的数据
			list->destroy(data);
	}
}

//尾插法
int dlist_ins_next(DList *list, DListElmt *element, const void *data)
{
	DListElmt *new_element;

	if (element == NULL && list->size != 0)
		return -1;

	if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
		return -1;
	new_element->data = (void *)data;

	if (list->size == 0)    //如果为链表头
	{
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}
	else
	{
		new_element->next = element->next;
		new_element->prev = element;

		if (element->next == NULL)
			list->tail = new_element;
		else
			element->next->prev = new_element;
		element->next = new_element;
	}
	list->size++;
	return 0;
}

int dlist_ins_prev(DList *list, DListElmt *element, const void *data)
{
	DListElmt *new_element;

	if (element == NULL  && list->size != 0)
		return -1;
	if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
		return -1;
	new_element->data = (void *)data;

	if (list->size == 0)
	{
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}
	else
	{
		new_element->next = element;
		new_element->prev = element->prev;
		if (element->prev == NULL)
			list->head = new_element;
		else
			element->prev->next = new_element;
		element->prev = new_element;
	}

	list->size++;
	return 0;
}
