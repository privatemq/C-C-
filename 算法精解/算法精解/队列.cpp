#include<stdlib.h>
#include "list.h"

//���б������һ�����࣬���Գ�ʼ�������룬ɾ���������������һ�¡�
//�������ʵ��
typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy

int queue_enqueue(Queue *queue, const void *data)
{
	//β��
	return list_ins_next(queue,list_tail(queue),data);
}

int queue_dequeue(Queue *queue, void **data)
{
	//ͷɾ
	return list_rem_next(queue, NULL, data);
}