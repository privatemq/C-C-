#include<stdlib.h>
#include "list.h"

//队列本身就是一个联编，所以初始化，插入，删除与链表操作基本一致。
//具体代码实现
typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy

int queue_enqueue(Queue *queue, const void *data)
{
	//尾插
	return list_ins_next(queue,list_tail(queue),data);
}

int queue_dequeue(Queue *queue, void **data)
{
	//头删
	return list_rem_next(queue, NULL, data);
}