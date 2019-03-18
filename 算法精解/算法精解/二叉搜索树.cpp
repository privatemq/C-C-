#include"bitree.h"
typedef BiTree BisTree;
//��ʼ��������������compare���ڱȽ�key1>key2?1:-1(= 0)
void bistree_init(BisTree *tree, void(*compare)(const void *key1, const void *key2),
	void(*destroy)(void *data));

//������
void bistree_destroy(BisTree *tree);

//����ڵ㣬�ɹ�����0��Ҫ����Ľڵ��Ѵ��ڷ���1������-1
int bistree_insert(BisTree *tree, const void *data);   //���Ӷ�lg n

//�Ƴ�����data�Ľڵ㣬�ɹ�����0������-1
int bistree_remove(BisTree *tree, const void *data);   //���Ӷ�lg n

//���Ҷ�Ӧdata�Ľڵ㣬�ɹ�����0������-1
int bistree_lookup(const BisTree *tree, void **data);  //���Ӷ�lg n

//����node�ڵ㿪ʼ�����֧
static void destroy_right(BisTree *tree, BiTreeNode *node);
static void destroy_left(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;
	if (tree->size == 0) return;   //������ڵ�Ϊ0 ��ֱ�ӷ���
	if (node == NULL)    //���nodeΪnull,�Ӹ��ڵ㿪ʼɾ��
		position = &tree->root;
	else
		position = &node->left;

	if (*position != NULL)
	{
		//�ݹ�����position��ʼ����������
		destroy_left(tree, *position);
		destroy_right(tree, *position);
		if (tree->destroy != NULL)
		{
			//��������������
		}

		//�ͷŽڵ�ռ䣬size--
		free((*position)->data);
		free(*position);
		*position = NULL;
		tree->size--;
	}
	return;
}

//����tree��node�ڵ㿪ʼ����data����
static int insert(BisTree *tree, BiTreeNode **node, const void *data)
{
	int cmpval, retval;
	if (bitree_is_eob(*node))   //���ָ���ڵ�û�����ҽڵ㣬����ڵ㿪ʼ����
	{
		return bitree_ins_left(tree, *node, data);
	}
	else
	{
		cmpval = tree->compare(data, (*node)->data);   //�Ƚ�node->data��data��ֵ��data��ʱ������������
		if (cmpval < 0)
		{
			if (bitree_is_eob((*node)->left)) //���node->leftΪ��
			{
				if (bitree_ins_left(tree, *node, data) != 0)
					return -1;
			}
			else
			{
				if ((retval = insert(tree, &((*node)->left), data)) != 0)
				{
					return retval;
				}
			}
		}
		else if(cmpval > 0)
		{
			if (bitree_is_eob((*node)->right)) //���node->leftΪ��
			{
				if (bitree_ins_right(tree, *node, data) != 0)
					return -1;
			}
			else
			{
				if ((retval = insert(tree, &((*node)->right), data)) != 0)
				{
					return retval;
				}
			}
		}
		else  //���node->data == data,��Ϊ���ݳ�ͻ
		{
			if (tree->destroy != NULL)
				tree->destroy((*node)->data);
			(*node)->data = data;   //�˴������ܽ�const data��ֵ��*node->data
		}
	}
	return 0;
}

//����tree��node�ڵ㿪ʼ����data����
static int lookup(BisTree *tree, BiTreeNode *node,const void **data)
{
	int cmpval, retval;
	if (bitree_is_eob(node)) //node�����ҽڵ㲻Ϊ��
	{
		return -1;
	}

	//�Ƚϲ���ڵ��Ҫ�������ݵ�ֵ
	cmpval = tree->compare(*data, node->data);

	if (cmpval < 0)   //�����������С��node->data,��node��������Ѱ��λ�ü����Ƚ�
		retval = lookup(tree, node->left, data);
	else if (cmpval > 0)  //����������ݴ���node->data,��node��������Ѱ��λ�ü����Ƚ�
		retval = lookup(tree, node->right, data);
	else   //����ʱ��ȡ������
	{ 
		*data = node->data;
	}

	return retval;
}

void bistree_init(BisTree *tree, int(*compare)(const void *key1, const void *key2), void(*destroy)(void *data))
{
	bitree_init(tree, destroy);
	tree->compare = compare;
	return;
}

void bistree_destroy(BisTree *tree)
{
	destroy_left(tree, NULL);
	memset(tree, 0, sizeof(BisTree));
	return;
}

int bistree_insert(BisTree *tree, const void *data)
{
	return insert(tree, &(tree->root), data);
}

int bistree_remove(BisTree *tree, const void *data)
{
	return lookup(tree, tree->root, &data);
}

int bistree_lookup(BisTree *tree, void **data)
{
	return lookup(tree, tree->root, data);
}