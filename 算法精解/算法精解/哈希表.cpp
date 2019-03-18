/*关于哈希表，通过看书与资料，有了一些自己的理解。
我更喜欢将哈希表比作是一个二维数组void chtbl[type][N]
同一数据类型的数据存储在一个一维数组里面，type表示不同类型的数据类型。
比如圆形，方形，心形，六边形代表四种类型的框，如果一个物体数据某种类型，
则将该物体放入对应的框中。而N则代表相应数据类型的个数，
而如何辨别该物体是什么形状，则需要一套方法，也就是哈希函数。
*/

//链式哈希表的实现
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

//新建链表
void list_init(List *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}
void list_destroy(List *haed);
typedef struct CHTBL_
{
	int buckets;     //数据类型的个数
	int (*h)(const void *key);  //自定义的哈希函数
	int (*match)(const void *key1, const void *key2);  //判断两个键是否匹配
	void (*destroy)(void *data);    //释放动态分配的内存
	int size;  //数据数量
	List *table;  //存储数据类型的数组
}CHTBL;

void chtbl_destroy(CHTBL *htbl)
{
	int i;
	for (i = 0; i < htbl->buckets; i++)
	{
		list_destory(&htbl->table[i]);
	}

	free(htbl->table);
	memset(htbl, 0, sizeof(CHTBL));
	return;
}
//哈希表初始化
int chtbl_init(CHTBL *htbl, int buckets, int(*h)(const void *key),
	int(*match)(const void *key1, const void *key2), void(*destroy)(void *data))
{
	int i;
	if ((htbl->table == (List *)malloc(sizeof(buckets * sizeof(List)))) == NULL)
	{
		return -1;
	}

	htbl->buckets = buckets;
	//初始化哈希表数据类型
	for (i = 0; i < htbl->buckets; i++)
	{
		list_init(&htbl->table[i], destroy);
	}

	htbl->h = h;
	htbl->match = match;
	htbl->destroy = destroy;
	htbl->size = 0;
	return 0;
}

//销毁哈希表
void chtbl_destroy(CHTBL *htbl)
{
	int i;
	for (i = 0; i < htbl->buckets; i++)
	{
		list_destroy(&htbl->table[i]);  //链表销毁函数
	}

	memset(htbl, 0, sizeof(CHTBL));
	return;
}

//查找元素，找到返回0，否则，返回-1
int chtbl_lookup(const CHTBL *htbl, void **data)
{
	ListElmt *element;
	int bucket;

	//直接对数据类型数取余，得到key
	bucket = htbl->h(*data) % htbl->buckets;

	//从获得数据类型的一维数组开始寻找data是否存在
	for (element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element))
	{
		if (htbl->match(*data, list_data(element)))
		{
			*data = list_data(element);
			return 0;
		}
	}
	return -1;
}

//插入数据
int chtbl_insert(CHTBL *htbl, const void *data)
{
	void *temp;
	int bucket, retval;

	temp = (void *)data;
	//如果数据存在，则返回1，不进行插入操作
	if (chtbl_lookup(htbl,&temp) == 0)   
		return 1;

	//获取传入数据的key(数据类型)
	bucket = htbl->h(data) % htbl->buckets;
	//如果数据不存在，将这个数据连接在这个数据类型的链表上
	if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
		htbl->size++;
	return retval;
}

int chtbl_remove(CHTBL *htbl, void **data)
{
	ListElmt *element,*prev;
	int bucket;

	bucket = htbl->h(data) % htbl->buckets;
	prev = NULL;

	//根据key寻找需要删除的数据
	for (element = list_head(&htbl->table[bucket]);
		element != NULL; element = list_next(element))
	{
		//如果找到数据
		if (htbl->match(*data, list_data(element)))
		{
			//开始删除，删除成功返回0，删除失败返回-1
			if (list_rem_next(&htbl->table[bucket], prev, data) == 0)
			{
				htbl->size--;
				return 0;
			}
			else
			{
				return -1;
			}
		}
		prev = element;
	}
	return -1;
}