////////////////˳��ջ
#define Stack_MAX 100

typedef struct Stack1
{
	int data[Stack_MAX];       //����
	int top;             //�α�
}Stack1;

void initStack(Stack1* s)
{
	s->top = -1;
}

//ջ�п�
int IsEmpty(Stack1 *s)
{
	if (s->top == -1)
		return 1;
	return 0;
}

//��ջ
int push(Stack1 *s, int x)
{
	if (s->top == Stack_MAX)
		return 0;
	else
		s->data[++(s->top)] = x;
	return 1;
}
//��ջ
int pop(Stack1 *s, int* x)
{
	if (s->top == -1)
		return 0;
	else
		*x = s->data[(s->top)--];
	return 1;
}


//��ջ�ڵ㶨��
typedef struct LNode
{
	int data;
	struct LNode *next;
}LNode;

//��ʼ��
void initLNode(LNode *l)
{
	l->data = -1;
	l->next = NULL;
}

//�п�
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
	a->next = l->next;   //����ͷ�巨��ͷ����Ľڵ�Ϊ������Ľڵ�
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
	l->next = l->next->next;   //ժ���ڵ�
	x = a->data;
	free(a);
	return x;  //�ڵ����ݳ�ջ
}


