#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ListElmt   //����ڵ�
{
	void *data;
	struct ListElmt  *next;
};

typedef struct List  //��������
{
	int size;
	int(*match)(const void *key1, const void *key2); //���캯��
	void(*destroy)(void *data);  //��������
	ListElmt *head;
	ListElmt *tail;
};

void list_init(List *list, void(*destroy)(void *data));     //�½�����
void list_destroy(List *list);                             //�Ƴ������е�Ԫ��
void list_size(List *list, void(*destroy)(void *data));    //������
int list_ins_next(List *list, ListElmt *element, const void *data);  //β��
int list_rem_next(List *list, ListElmt *element, void **data);   //�Ƴ�

#define list_is_head(list,element) ((element) == (list)->head ? 1:0)
#define list_is_tail(element) ((element)->next == NULL ? 1:0)