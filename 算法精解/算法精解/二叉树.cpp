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
	int size;   //树种节点的个数
	int(*compare)(const void *key1, const void *key2);
	void(*destroy)(void *data);    //析构函数
	BiTreeNode *root;             //指向根节点的指针
}BiTree;

#define bitree_size(tree) ((tree)->size)      //求二叉树节点个数
#define bitree_root(tree) ((tree)->root)      //返回根节点
#define bitree_is_eob(node) ((node) == NULL)  //判断节点所标识的节点是否为二叉树中某个分支的结束
#define bitree_is_leaf(node) ((node->left == NULL && (node)->right == NULL))  //判断节点是否非二叉树中叶子节点
#define bitree_data(node) ((node)->data)     //获取节点数据
#define bitree_left(node) ((node)->left)     //获取节点的左节点
#define bitree_right(node) ((node)->right)   //获取节点的右节点

//初始化二叉树
void bitree_init(BiTree *tree, void(*destroy)(void *data))
{
	tree->size = 0;
	tree->destroy = destroy;
	tree->root = NULL;
	return;
}

//析构函数
void bitree_destroy(BiTree *tree)
{
	//采用左节点消除，释放空间
	bitree_rem_left(tree, NULL);

	memset(tree, 0, sizeof(BiTree));        //节点赋空
	return;
}

//指定节点位置插入左节点
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
	BiTreeNode *new_node, **position;

	//如果指定节点不存在
	if (node == NULL)
	{
		//如果二叉树不为空，则插入失败
		if (bitree_size(tree) > 0)
			return -1;
		//如果二叉树为空,从根节点开始插入
		position = &tree->root;
	}
	else
	{
		//如果指定节点已经有左节点,插入左节点
		if (bitree_left(node) != NULL)
			return -1;
		position = &node->left;
	}

	if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;

	//开始添加左节点
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;

	tree->size++;
	return 0;
}

//指定节点位置插入右节点,与插入左节点逻辑基本一致
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

//从指定节点开始删除左子树
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;
	//如果树为空，则删除失败
	if (bitree_size(tree) == 0)
		return;
	//如果指定节点为nULL，则从根节点开始删除，否则从左节点开始删除
	if (node == NULL)
	{
		position = &tree->root;
	}
	else
	{
		position = &node->left;
	}

	//如果节点不为空，开始递归到最底层节点
	if (*position != NULL)
	{
		bitree_rem_left(tree, *position);
		bitree_rem_right(tree, *position);

		//从最底层开始递归释放
		if (tree->destroy != NULL)
			tree->destroy((*position)->data);
		free(*position);
		*position = NULL;

		tree->size--;
	}
	return;
}

//将两颗二叉树合并为单颗二叉树
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
	//初始化merge
	bitree_init(merge, left->destroy);

	//插入根节点
	if (bitree_ins_left(merge, NULL, data) != 0)
	{
		bitree_destroy(merge);
		return -1;
	}

	//将另一条二叉树的左右节点，复制给merge
	bitree_root(merge)->left = bitree_root(left);
	bitree_root(merge)->right = bitree_root(right);

	merge->size = merge->size + bitree_size(left) + bitree_size(right);

	left->root = NULL;
	left->size = 0;
	right->root = NULL;
	right->size = 0;

	return 0;
}

//先序遍历
int preorder(const BiTreeNode *node, List *list)
{
	//如果第一个节点不为空
	if (!bitree_is_eob(node))
	{
		//链表list中先插入根节点
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;
		//如果左节点不为空，插入左节点
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (preorder(node->left, list) != 0)
				return -1;
		}
		//如果右节点不为空，插入右节点
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (preorder(node->right, list) != 0)
				return -1;
		}
	}
}

//中序遍历
int inorder(const BiTreeNode *node, List *list)
{
	//如果第一个节点不为空
	if (!bitree_is_eob(node))
	{
		//如果左节点不为空，插入左节点
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (inorder(node->left, list) != 0)
				return -1;
		}

		//链表list中先插入根节点
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;

		//如果右节点不为空，插入右节点
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (inorder(node->right, list) != 0)
				return -1;
		}
	}
}

//后序遍历
int postorder(const BiTreeNode *node, List *list)
{
	//如果第一个节点不为空
	if (!bitree_is_eob(node))
	{
		//链表list中先插入根节点
		if (list_ins_next(list, list->tail, bitree_data(node)) != 0)
			return -1;

		//如果右节点不为空，插入右节点
		if (!bitree_is_eob(bitree_right(node)))
		{
			if (postorder(node->right, list) != 0)
				return -1;
		}

		//如果左节点不为空，插入左节点
		if (!bitree_is_eob(bitree_left(node)))
		{
			if (postorder(node->left, list) != 0)
				return -1;
		}
	}
}