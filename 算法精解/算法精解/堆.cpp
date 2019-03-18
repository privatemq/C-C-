#include<stdio.h>
#include<stdlib.h>

typedef struct Heap_
{
	int size;
	int(*compare)(const void *key1, const void *key2);
	void(*destroy)(void *data);
	void **tree;
}Heap;

#define heap_parent(npos) ((int)(((npos)-1)/2))   //父节点
#define heap_left(nops) (((nops)*2)+1)           //左节点
#define heap_right (((nops)*2)+2)               //右节点
#define heap_size(heap) (heap->size)

//初始化堆
void heap_init(Heap *heap, int(*compare)(const void *key1, const void *key2), void(*destroy)(void *data))
{
	heap->size = 0;
	heap->compare = compare;
	heap->destroy = destroy;
	heap->tree = NULL;
	return;
}

//销毁堆
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

//插入元素成功，返回0 ，否则返回-1       复杂度 lg（n）
int heap_insert(Heap *heap, const void *data)
{
	void **temp;
	int ipos, ppos;

	// 添加节点时，先使用realloct在原本的空间上添加空间
	if((temp = (void **)realloc(heap->tree,(heap_size(heap)+1)*sizeof(void *))) == NULL)
	{
		//添加空间失败
		return  -1;
	}
	else
	{
		//成功时，将新的空间temp赋给data
		heap->tree = temp;
	}
	//将要插入的数据添加在数组的最后一位
	heap->tree[heap_size(heap)] = (void *)data;

	ipos = heap_size(heap);   //新插入节点的位置
	ppos = heap_parent(ipos); //新插入节点的父节点

	//如果新节点存在父节点，且父节点小于新节点，则交换两个节点的位置
	while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0)
	{
		*temp = heap->tree[ppos];
		heap->tree[ppos] = heap->tree[ipos];
		heap->tree[ipos] = *temp;
		//向上层继续查找交换
		ipos = ppos;
		ppos = heap_parent(ipos);
	}
	heap->size++;
	return 0;
}

//释放节点，成功0，失败-1  lg(n)
int heap_extract(Heap *heap, void **data)
{
	void *save, **temp;
	int ipos, lpos, rpos, mpos;

	if (heap_size(heap) == 0)
		return -1;
	*data = heap->tree[0];  //取根节点，即最大值
	save = heap->tree[heap_size(heap) - 1]; //取数组最后一位，即堆最后一位
	//如果存在节点大于0
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
		//存在数据为0
		free(heap->tree);
		heap->tree = NULL;
		heap->size = 0;
		return 0;
	}

	//取最大值后，调节堆的数据存放，调节堆的最大值
	heap->tree[0] = save;   //最后一个节点变为根节点
	ipos = 0;
	lpos = heap_left(ipos);
	rpos = heap_right(ipos);

	while (1)
	{
		lpos = heap_left(ipos);
		rpos = heap_right(ipos);
		//如果根节点的左节点存在，且左节点数据大于根节点数据
		if(lpos < heap_size(heap) && heap->compare(heap->tree[lpos],heap->tree[ipos]) > 0)
		{
			mpos = lpos;  
		}
		else
		{
			mpos = ipos;
		}

		//经过上面的判断，大的那一个节点再和右节点比较
		if (rpos < heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0)
		{
			mpos = rpos;
		}
		//两次比较之后，最大节点的位置为mpos
		if (mpos == ipos)
		{
			//如果根节点大于左右节点，则成立，退出循环
			break;
		}
		else
		{
			//否则，继续交换根节点和最大节点的位置，循环查找
			*temp = heap->tree[mpos];
			heap->tree[mpos] = heap->tree[ipos];
			heap->tree[ipos] = *temp;

			ipos = mpos;
		}
	}
	return 0;
}

