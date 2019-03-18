#include"Heap.h"

typedef Heap PQueue;

#define pqueue_init heap_init
#define pqueue_destroy heap_destroy
#define pqueue_insert heap_insert
#define pqueue_extract heap_extract
#define pqueue_peek(pqueue) ((pqueue)->tree == NULL ? NULL:(pqueue)->tree[0])
#define pqueue_size heap_size


//�����ּ�����ʵ��
int get_parcel(PQueue *parcels, Parcel *parcel)
{
	Parcel *data;
	//���û�а������򷵻�-1
	if (pqueue_size(parcels) == 0)
		return -1;
	else
	{
		//���δȡ���������
		if (pqueue_extract(parcels, (void **)*data) != 0)
			return -1;
		else
		{
			//ȡ�����������ݡ�
			memcpy(parcel,data,sizeof(Parcel));
			//ȡ��������data�����ڿռ䡣֮����ʹ��ʱ��Ӧ�ͷſռ�
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
	//�����ȼ����ݲ��뵽��Ӧ���ȼ�������
	if (pqueue_insert(parcels, data) != 0)
		return -1;
	return 0;
}