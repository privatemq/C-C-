#pragma once
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
void heap_init(Heap *heap, int(*compare)(const void *key1, const void *key2), void(*destroy)(void *data));

//���ٶ�
void heap_destroy(Heap *heap);

//����Ԫ�سɹ�������0 �����򷵻�-1       ���Ӷ� lg��n��
int heap_insert(Heap *heap, const void *data);

//�ͷŽڵ㣬�ɹ�0��ʧ��-1  lg(n)
int heap_extract(Heap *heap, void **data);
