#include"bitree.h"
typedef BiTree BisTree;
//初始化二叉搜索树，compare用于比较key1>key2?1:-1(= 0)
void bistree_init(BisTree *tree, void(*compare)(const void *key1, const void *key2),
	void(*destroy)(void *data));

//销毁树
void bistree_destroy(BisTree *tree);

//插入节点，成功返回0，要插入的节点已存在返回1，否则-1
int bistree_insert(BisTree *tree, const void *data);   //复杂度lg n

//移除等于data的节点，成功返回0，否则-1
int bistree_remove(BisTree *tree, const void *data);   //复杂度lg n

//查找对应data的节点，成功返回0，否则-1
int bistree_lookup(const BisTree *tree, void **data);  //复杂度lg n

//销毁node节点开始的左分支
static void destroy_right(BisTree *tree, BiTreeNode *node);
static void destroy_left(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode **position;
	if (tree->size == 0) return;   //如果树节点为0 ，直接返回
	if (node == NULL)    //如果node为null,从根节点开始删除
		position = &tree->root;
	else
		position = &node->left;

	if (*position != NULL)
	{
		//递归消除position开始的左右子树
		destroy_left(tree, *position);
		destroy_right(tree, *position);
		if (tree->destroy != NULL)
		{
			//调用析构函数；
		}

		//释放节点空间，size--
		free((*position)->data);
		free(*position);
		*position = NULL;
		tree->size--;
	}
	return;
}

//在树tree的node节点开始插入data数据
static int insert(BisTree *tree, BiTreeNode **node, const void *data)
{
	int cmpval, retval;
	if (bitree_is_eob(*node))   //如果指定节点没有左右节点，从左节点开始插入
	{
		return bitree_ins_left(tree, *node, data);
	}
	else
	{
		cmpval = tree->compare(data, (*node)->data);   //比较node->data和data的值，data大时，右子树插入
		if (cmpval < 0)
		{
			if (bitree_is_eob((*node)->left)) //如果node->left为空
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
			if (bitree_is_eob((*node)->right)) //如果node->left为空
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
		else  //如果node->data == data,则为数据冲突
		{
			if (tree->destroy != NULL)
				tree->destroy((*node)->data);
			(*node)->data = data;   //此处，不能讲const data赋值给*node->data
		}
	}
	return 0;
}

//在树tree的node节点开始查找data数据
static int lookup(BisTree *tree, BiTreeNode *node,const void **data)
{
	int cmpval, retval;
	if (bitree_is_eob(node)) //node的左右节点不为空
	{
		return -1;
	}

	//比较插入节点和要插入数据的值
	cmpval = tree->compare(*data, node->data);

	if (cmpval < 0)   //如果插入数据小于node->data,在node的左子树寻找位置继续比较
		retval = lookup(tree, node->left, data);
	else if (cmpval > 0)  //如果插入数据大于node->data,在node的右子树寻找位置继续比较
		retval = lookup(tree, node->right, data);
	else   //等于时，取出数据
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