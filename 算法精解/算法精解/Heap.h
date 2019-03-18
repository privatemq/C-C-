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

#define heap_parent(npos) ((int)(((npos)-1)/2))   //父节点
#define heap_left(nops) (((nops)*2)+1)           //左节点
#define heap_right (((nops)*2)+2)               //右节点
#define heap_size(heap) (heap->size)

//初始化堆
void heap_init(Heap *heap, int(*compare)(const void *key1, const void *key2), void(*destroy)(void *data));

//销毁堆
void heap_destroy(Heap *heap);

//插入元素成功，返回0 ，否则返回-1       复杂度 lg（n）
int heap_insert(Heap *heap, const void *data);

//释放节点，成功0，失败-1  lg(n)
int heap_extract(Heap *heap, void **data);
