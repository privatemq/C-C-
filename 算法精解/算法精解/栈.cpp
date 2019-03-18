////////////////顺序栈
#define Stack_MAX 100

typedef struct Stack1
{
	int data[Stack_MAX];       //数据
	int top;             //游标
}Stack1;

void initStack(Stack1* s)
{
	s->top = -1;
}

//栈判空
int IsEmpty(Stack1 *s)
{
	if (s->top == -1)
		return 1;
	return 0;
}

//进栈
int push(Stack1 *s, int x)
{
	if (s->top == Stack_MAX)
		return 0;
	else
		s->data[++(s->top)] = x;
	return 1;
}
//出栈
int pop(Stack1 *s, int* x)
{
	if (s->top == -1)
		return 0;
	else
		*x = s->data[(s->top)--];
	return 1;
}


//链栈节点定义
typedef struct LNode
{
	int data;
	struct LNode *next;
}LNode;

//初始化
void initLNode(LNode *l)
{
	l->data = -1;
	l->next = NULL;
}

//判空
int IsEmpty(LNode*l)
{
	if (l->next == NULL)
		return 1;
	return 0;
}

int push(LNode *l, int data)
{
	LNode *a;
	if ((a = (LNode*)malloc(sizeof(a))) == NULL)
		return 0;
	a->data = data;
	a->next = l->next;   //采用头插法，头结点后的节点为最后进入的节点
	l->next = a;
	return 1;
}

int pop(LNode *l)
{
	LNode *a;
	int x;
	if (l->next == NULL)
		return 0;
	a = l->next;
	l->next = l->next->next;   //摘除节点
	x = a->data;
	free(a);
	return x;  //节点数据出栈
}


