/*���ڹ�ϣ��ͨ�����������ϣ�����һЩ�Լ�����⡣
�Ҹ�ϲ������ϣ�������һ����ά����void chtbl[type][N]
ͬһ�������͵����ݴ洢��һ��һά�������棬type��ʾ��ͬ���͵��������͡�
����Բ�Σ����Σ����Σ������δ����������͵Ŀ����һ����������ĳ�����ͣ�
�򽫸���������Ӧ�Ŀ��С���N�������Ӧ�������͵ĸ�����
����α���������ʲô��״������Ҫһ�׷�����Ҳ���ǹ�ϣ������
*/

//��ʽ��ϣ���ʵ��
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

//�½�����
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
	int buckets;     //�������͵ĸ���
	int (*h)(const void *key);  //�Զ���Ĺ�ϣ����
	int (*match)(const void *key1, const void *key2);  //�ж��������Ƿ�ƥ��
	void (*destroy)(void *data);    //�ͷŶ�̬������ڴ�
	int size;  //��������
	List *table;  //�洢�������͵�����
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
//��ϣ���ʼ��
int chtbl_init(CHTBL *htbl, int buckets, int(*h)(const void *key),
	int(*match)(const void *key1, const void *key2), void(*destroy)(void *data))
{
	int i;
	if ((htbl->table == (List *)malloc(sizeof(buckets * sizeof(List)))) == NULL)
	{
		return -1;
	}

	htbl->buckets = buckets;
	//��ʼ����ϣ����������
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

//���ٹ�ϣ��
void chtbl_destroy(CHTBL *htbl)
{
	int i;
	for (i = 0; i < htbl->buckets; i++)
	{
		list_destroy(&htbl->table[i]);  //�������ٺ���
	}

	memset(htbl, 0, sizeof(CHTBL));
	return;
}

//����Ԫ�أ��ҵ�����0�����򣬷���-1
int chtbl_lookup(const CHTBL *htbl, void **data)
{
	ListElmt *element;
	int bucket;

	//ֱ�Ӷ�����������ȡ�࣬�õ�key
	bucket = htbl->h(*data) % htbl->buckets;

	//�ӻ���������͵�һά���鿪ʼѰ��data�Ƿ����
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

//��������
int chtbl_insert(CHTBL *htbl, const void *data)
{
	void *temp;
	int bucket, retval;

	temp = (void *)data;
	//������ݴ��ڣ��򷵻�1�������в������
	if (chtbl_lookup(htbl,&temp) == 0)   
		return 1;

	//��ȡ�������ݵ�key(��������)
	bucket = htbl->h(data) % htbl->buckets;
	//������ݲ����ڣ��������������������������͵�������
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

	//����keyѰ����Ҫɾ��������
	for (element = list_head(&htbl->table[bucket]);
		element != NULL; element = list_next(element))
	{
		//����ҵ�����
		if (htbl->match(*data, list_data(element)))
		{
			//��ʼɾ����ɾ���ɹ�����0��ɾ��ʧ�ܷ���-1
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