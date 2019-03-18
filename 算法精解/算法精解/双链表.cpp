#include<stdlib.h>
#include<string.h>
typedef struct DListElmt    //�ڵ�����
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

#define dlist_is_head(element) ((element)->perv == NULL ? 1:0)  //�Ƿ���ͷ
#define dlist_is_tail(element) ((element)->next == NULL ? 1:0)  //�Ƿ���β

//��������
void dlist_init(DList *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

//ɾ���ڵ�
int dlist_remove(DList *list, DListElmt *element, void **data)
{
	if (element == NULL || list->size == 0)
		return 1;
	*data = element->data;

	if (element == list->head)       //�����ͷ�ڵ㣬���пա�
	{
		if (list->head == NULL)    //Ϊ���������һ���ڵ㣬�������
			list->tail = NULL;
		else
			element->next->prev = NULL;  //�ǿգ���ժ���ڵ㣬׼��ɾ��
	}
	else   //�������ͷ��㣬ժ����ɾ��
	{
		element->prev->next = element->next; //ɾ���ڵ��ǰһ��ָ��ɾ���ڵ�ĺ�һ��
		if (element->next == NULL)    //�ж��Ƿ�Ϊ���һ���ڵ�
			list->tail = element->prev;
		else
			element->next->prev = element->prev; //ɾ���ڵ�ĺ�һ��ָ��ɾ���ڵ��ǰһ��
	}

	free(element);
	list->size--;
	return 0;
}

//ɾ������
void dlist_destroy(DList *list)
{
	void *data;
	while (list->size > 0)
	{
		if (dlist_remove(list, list->tail, (void **)&data) == 0
			&& list->destroy != NULL)
			//����ڵ�ɾ���ɹ�����ɾ���ڵ��Ӧ�洢������
			list->destroy(data);
	}
}

//β�巨
int dlist_ins_next(DList *list, DListElmt *element, const void *data)
{
	DListElmt *new_element;

	if (element == NULL && list->size != 0)
		return -1;

	if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
		return -1;
	new_element->data = (void *)data;

	if (list->size == 0)    //���Ϊ����ͷ
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
