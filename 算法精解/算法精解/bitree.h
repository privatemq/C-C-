#pragma once
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
void bitree_init(BiTree *tree, void(*destroy)(void *data));
//析构函数
void bitree_destroy(BiTree *tree);
//指定节点位置插入左节点
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
//指定节点位置插入右节点,与插入左节点逻辑基本一致
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);

//从指定节点开始删除左子树
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
void bitree_rem_left(BiTree *tree, BiTreeNode *node);
//将两颗二叉树合并为单颗二叉树
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);
//先序遍历
int preorder(const BiTreeNode *node, List *list);
//中序遍历
int inorder(const BiTreeNode *node, List *list);
//后序遍历
int postorder(const BiTreeNode *node, List *list);