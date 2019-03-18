#include<stdlib.h>
#include<string.h>
#include"list.h"

typedef struct BiTreeNode_
{
	void *data;
	struct BiTreeNode_ *left;
	struct BiTreeNode_ *right;
}BiTreeNode;

typedef struct BiTree_
{
	int size;   //���ֽڵ�ĸ���
	int(*compare)(const void *key1, const void *key2);
	void(*destroy)(void *data);    //��������
	BiTreeNode *root;             //ָ����ڵ��ָ��
}BiTree;

#define bitree_size(tree) ((tree)->size)      //��������ڵ����
#define bitree_root(tree) ((tree)->root)      //���ظ��ڵ�
#define bitree_is_eob(node) ((node) == NULL)  //�жϽڵ�����ʶ�Ľڵ��Ƿ�Ϊ��������ĳ����֧�Ľ���
#define bitree_is_leaf(node) ((node->left == NULL && (node)->right == NULL))  //�жϽڵ��Ƿ�Ƕ�������Ҷ�ӽڵ�
#define bitree_data(node) ((node)->data)     //��ȡ�ڵ�����
#define bitree_left(node) ((node)->left)     //��ȡ�ڵ����ڵ�
#define bitree_right(node) ((node)->right)   //��ȡ�ڵ���ҽڵ�

//��ʼ��������
void bitree_init(BiTree *tree, void(*destroy)(void *data))
{
	tree->size = 0;
	tree->destroy = destroy;
	tree->root = NULL;
	return;
}

//��������
void bitree_destroy(BiTree *tree)
{
	//������ڵ��������ͷſռ�
	bitree_rem_left(tree, NULL);

	memset(tree, 0, sizeof(BiTree));        //�ڵ㸳��
	return;
}

//ָ���ڵ�λ�ò�����ڵ�
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
	BiTreeNode *new_node, **position;

	//���ָ���ڵ㲻����
	if (node == NULL)
	{
		//�����������Ϊ�գ������ʧ��
		if (bitree_size(tree) > 0)
			return -1;
		//���������Ϊ��,�Ӹ��ڵ㿪ʼ����
		position = &tree->root;
	}
	else
	{
		//���ָ���ڵ��Ѿ�����ڵ�,������ڵ�
		if (bitree_left(node) != NULL)
			return -1;
		position = &node->left;
	}

	if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;

	//��ʼ�����ڵ�
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;

	tree->size++;
	return 0;
}

//ָ���ڵ�λ�ò����ҽڵ�,�������ڵ��߼�����һ��
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
	BiTreeNode *new_node, **position;

	if (node == NULL)
	{
		if (bitree_size(tree) > 0)
			return -1;
		position = &tree->root;
	}
	else
	{
		if (bitree_right(node) != NULL)
			return -1;
		position = &node->right;
	}

	if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;

	tree->size++;
	return 0;
}

//��ָ���ڵ㿪ʼɾ��������
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;
	//�����Ϊ�գ���ɾ��ʧ��
	if (bitree_size(tree) == 0)
		return;
	//���ָ���ڵ�ΪnULL����Ӹ��ڵ㿪ʼɾ�����������ڵ㿪ʼɾ��
	if (node == NULL)
	{
		position = &tree->root;
	}
	else
	{
		position = &node->left;
	}

	//����ڵ㲻Ϊ�գ���ʼ�ݹ鵽��ײ�ڵ�
	if (*position != NULL)
	{
		bitree_rem_left(tree, *position);
		bitree_rem_right(tree, *position);

		//����ײ㿪ʼ�ݹ��ͷ�
		if (tree->destroy != NULL)
			tree->destroy((*position)->data);
		free(*position);
		*position = NULL;

		tree->size--;
	}
	return;
}

//�����Ŷ������ϲ�Ϊ���Ŷ�����
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
	//��ʼ��merge
	bitree_init(merge, left->destroy);

	//������ڵ�
	if (bitree_ins_left(merge, NULL, data) != 0)
	{
		bitree_destroy(merge);
		return -1;
	}

	//����һ�������������ҽڵ㣬���Ƹ�merge
	bitree_root(merge)->left = bitree_root(left);
	bitree_root(merge)->right = bitree_root(right);

	merge->size = merge->size + bitree_size(left) + bitree_size(right);

	left->root = NULL;
	left->size = 0;
	right->root = NULL;
	right->size = 0;

	return 0;
}

//�������
int preorder(const BiTreeNode *node, List *list)
{
	//�����һ���ڵ㲻Ϊ��
	if (!bitree_is_eob(node))
	{
		//����list���Ȳ�����ڵ�
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;
		//�����ڵ㲻Ϊ�գ�������ڵ�
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (preorder(node->left, list) != 0)
				return -1;
		}
		//����ҽڵ㲻Ϊ�գ������ҽڵ�
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (preorder(node->right, list) != 0)
				return -1;
		}
	}
}

//�������
int inorder(const BiTreeNode *node, List *list)
{
	//�����һ���ڵ㲻Ϊ��
	if (!bitree_is_eob(node))
	{
		//�����ڵ㲻Ϊ�գ�������ڵ�
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (inorder(node->left, list) != 0)
				return -1;
		}

		//����list���Ȳ�����ڵ�
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;

		//����ҽڵ㲻Ϊ�գ������ҽڵ�
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (inorder(node->right, list) != 0)
				return -1;
		}
	}
}

//�������
int postorder(const BiTreeNode *node, List *list)
{
	//�����һ���ڵ㲻Ϊ��
	if (!bitree_is_eob(node))
	{
		//����list���Ȳ�����ڵ�
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;

		//����ҽڵ㲻Ϊ�գ������ҽڵ�
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (postorder(node->right, list) != 0)
				return -1;
		}

		//�����ڵ㲻Ϊ�գ�������ڵ�
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (postorder(node->left, list) != 0)
				return -1;
		}
	}
}