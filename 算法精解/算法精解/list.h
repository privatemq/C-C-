#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ListElmt   //定义节点
{
	void *data;
	struct ListElmt  *next;
};

typedef struct List  //定义链表
{
	int size;
	int(*match)(const void *key1, const void *key2); //构造函数
	void(*destroy)(void *data);  //析构函数
	ListElmt *head;
	ListElmt *tail;
};

void list_init(List *list, void(*destroy)(void *data));     //新建链表
void list_destroy(List *list);                             //移除链表中的元素
void list_size(List *list, void(*destroy)(void *data));    //链表长度
int list_ins_next(List *list, ListElmt *element, const void *data);  //尾插
int list_rem_next(List *list, ListElmt *element, void **data);   //移除

#define list_is_head(list,element) ((element) == (list)->head ? 1:0)
#define list_is_tail(element) ((element)->next == NULL ? 1:0)