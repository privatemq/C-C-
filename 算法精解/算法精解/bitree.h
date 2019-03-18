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
void bitree_init(BiTree *tree, void(*destroy)(void *data));
//��������
void bitree_destroy(BiTree *tree);
//ָ���ڵ�λ�ò�����ڵ�
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
//ָ���ڵ�λ�ò����ҽڵ�,�������ڵ��߼�����һ��
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);

//��ָ���ڵ㿪ʼɾ��������
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
void bitree_rem_left(BiTree *tree, BiTreeNode *node);
//�����Ŷ������ϲ�Ϊ���Ŷ�����
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);
//�������
int preorder(const BiTreeNode *node, List *list);
//�������
int inorder(const BiTreeNode *node, List *list);
//�������
int postorder(const BiTreeNode *node, List *list);