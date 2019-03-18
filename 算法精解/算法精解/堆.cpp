#include<stdio.h>
#include<stdlib.h>

typedef struct Heap_
{
	int size;
	int(*compare)(const void *key1, const void *key2);
	void(*destroy)(void *data);
	void **tree;
}Heap;

#define heap_parent(npos) ((int)(((npos)-1)/2))   //���ڵ�
#define heap_left(nops) (((nops)*2)+1)           //��ڵ�
#define heap_right (((nops)*2)+2)               //�ҽڵ�
#define heap_size(heap) (heap->size)

//��ʼ����
void heap_init(Heap *heap, int(*compare)(const void *key1, const void *key2), void(*destroy)(void *data))
{
	heap->size = 0;
	heap->compare = compare;
	heap->destroy = destroy;
	heap->tree = NULL;
	return;
}

//���ٶ�
void heap_destroy(Heap *heap)
{
	int i;
	if (heap->destroy != NULL)
	{
		for (i = 0; i < heap_size(heap); i++)
		{
			heap->destroy(heap->tree[i]);
		}
	}

	free(heap->tree);
	memset(heap, 0, sizeof(Heap));
	return;
}

//����Ԫ�سɹ�������0 �����򷵻�-1       ���Ӷ� lg��n��
int heap_insert(Heap *heap, const void *data)
{
	void **temp;
	int ipos, ppos;

	// ��ӽڵ�ʱ����ʹ��realloct��ԭ���Ŀռ�����ӿռ�
	if((temp = (void **)realloc(heap->tree,(heap_size(heap)+1)*sizeof(void *))) == NULL)
	{
		//��ӿռ�ʧ��
		return  -1;
	}
	else
	{
		//�ɹ�ʱ�����µĿռ�temp����data
		heap->tree = temp;
	}
	//��Ҫ����������������������һλ
	heap->tree[heap_size(heap)] = (void *)data;

	ipos = heap_size(heap);   //�²���ڵ��λ��
	ppos = heap_parent(ipos); //�²���ڵ�ĸ��ڵ�

	//����½ڵ���ڸ��ڵ㣬�Ҹ��ڵ�С���½ڵ㣬�򽻻������ڵ��λ��
	while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0)
	{
		*temp = heap->tree[ppos];
		heap->tree[ppos] = heap->tree[ipos];
		heap->tree[ipos] = *temp;
		//���ϲ�������ҽ���
		ipos = ppos;
		ppos = heap_parent(ipos);
	}
	heap->size++;
	return 0;
}

//�ͷŽڵ㣬�ɹ�0��ʧ��-1  lg(n)
int heap_extract(Heap *heap, void **data)
{
	void *save, **temp;
	int ipos, lpos, rpos, mpos;

	if (heap_size(heap) == 0)
		return -1;
	*data = heap->tree[0];  //ȡ���ڵ㣬�����ֵ
	save = heap->tree[heap_size(heap) - 1]; //ȡ�������һλ���������һλ
	//������ڽڵ����0
	if (heap_size(heap) - 1 > 0)
	{
		if ((temp = (void **)realloc(heap->tree, (heap_size(heap) - 1) * sizeof(void **))) == NULL)
		{
			return -1;
		}
		else
		{
			heap->tree = temp;
		}
		heap->size--;
	}
	else
	{
		//��������Ϊ0
		free(heap->tree);
		heap->tree = NULL;
		heap->size = 0;
		return 0;
	}

	//ȡ���ֵ�󣬵��ڶѵ����ݴ�ţ����ڶѵ����ֵ
	heap->tree[0] = save;   //���һ���ڵ��Ϊ���ڵ�
	ipos = 0;
	lpos = heap_left(ipos);
	rpos = heap_right(ipos);

	while (1)
	{
		lpos = heap_left(ipos);
		rpos = heap_right(ipos);
		//������ڵ����ڵ���ڣ�����ڵ����ݴ��ڸ��ڵ�����
		if(lpos < heap_size(heap) && heap->compare(heap->tree[lpos],heap->tree[ipos]) > 0)
		{
			mpos = lpos;  
		}
		else
		{
			mpos = ipos;
		}

		//����������жϣ������һ���ڵ��ٺ��ҽڵ�Ƚ�
		if (rpos < heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0)
		{
			mpos = rpos;
		}
		//���αȽ�֮�����ڵ��λ��Ϊmpos
		if (mpos == ipos)
		{
			//������ڵ�������ҽڵ㣬��������˳�ѭ��
			break;
		}
		else
		{
			//���򣬼����������ڵ�����ڵ��λ�ã�ѭ������
			*temp = heap->tree[mpos];
			heap->tree[mpos] = heap->tree[ipos];
			heap->tree[ipos] = *temp;

			ipos = mpos;
		}
	}
	return 0;
}

