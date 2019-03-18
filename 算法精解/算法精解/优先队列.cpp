#include"Heap.h"

typedef Heap PQueue;

#define pqueue_init heap_init
#define pqueue_destroy heap_destroy
#define pqueue_insert heap_insert
#define pqueue_extract heap_extract
#define pqueue_peek(pqueue) ((pqueue)->tree == NULL ? NULL:(pqueue)->tree[0])
#define pqueue_size heap_size


//包裹分拣函数的实现
int get_parcel(PQueue *parcels, Parcel *parcel)
{
	Parcel *data;
	//如果没有包裹，则返回-1
	if (pqueue_size(parcels) == 0)
		return -1;
	else
	{
		//如果未取到最大数据
		if (pqueue_extract(parcels, (void **)*data) != 0)
			return -1;
		else
		{
			//取到最优先数据】
			memcpy(parcel,data,sizeof(Parcel));
			//取出的数据data，存在空间。之后不再使用时，应释放空间
			free(data);
		}
	}
	return 0;
}

int put_parcel(PQueue *parcels, const Parcel *parcel)
{
	Parcel *data;
	if ((data = (Parcel*)malloc(sizeof(Parcel))) == NULL)
	{
		return -1;
	}

	memcpy(data, parcel, sizeof(Parcel));
	//将优先级数据插入到对应优先级队列中
	if (pqueue_insert(parcels, data) != 0)
		return -1;
	return 0;
}