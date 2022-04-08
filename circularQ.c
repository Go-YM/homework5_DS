/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */
#include <stdio.h>
#include <stdlib.h>
//<stdio.h>�� <stdlib.h>�� ������ ���� �ҽ��� Ȯ���ϵ��� ����

#define MAX_QUEUE_SIZE 4    
//MAX_QUEUE_SIZE�� ����� ���� �� 4 ���� 

typedef char element;
//element ����ü ����

typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;    //front�� rear ����
}QueueType;
//QueueType ����ü ����

//�Լ� ����
QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
    //QueueType ��ü *cQ ���� �� ť ����
	QueueType *cQ = createQueue();
	element data;   //element ��ü data ����
	char command;   //����ڷκ��� ����� �Է¹��� ���� command ����

    //q or Q�� �Է� �ɶ����� �ݺ��ϴ� �ݺ���
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	//����ڷκ��� Command�� �Է¹���

		switch(command){
		case 'i': case 'I':		//Command�� i or I �϶�
			data = getElement();	
			enQueue(cQ, data);		
			break;

		case 'd': case 'D':		//Command�� d or D �϶�
			deQueue(cQ, &data);
			break;

		case 'p': case 'P': 	//Command�� p or P �϶�
			printQ(cQ);
			break;

		case 'b': case 'B':		//Command�� b or B �϶�
			debugQ(cQ);
			break;

		case 'q': case 'Q':		//Command�� q or Q �϶�
   	        freeQueue(cQ);
			break;

		default:				//Command�� �߸� �Է� ������
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;   //1 ��ȯ
}

QueueType *createQueue()
{//Queue�� �����ϴ� �Լ�
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;	//front�� 0���� �ʱ�ȭ
	cQ->rear = 0;	//rear�� 0���� �ʱ�ȭ
	return cQ;
}

//rear�� front�� ������ empty rear�� front�� ���� �ֶ� full 
 
int freeQueue(QueueType *cQ)
{//Queue�� �޸� �Ҵ��� free���ִ� �Լ�
    if(cQ == NULL) return 1;    //cQ�� ��������� 1 ��ȯ
    free(cQ);   //cQ�� ������� ������ free
    return 1;   //1 ��ȯ
}

element getElement()
{//����ڷκ��� element�� �޴� �Լ�
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{//Queue�� ��� �ִ��� Ȯ���ϴ� �Լ�
    if((cQ->front) == (cQ->rear))	//ť�� ���������
    {
		printf("Already Queue is Full.\n");		//��� ���
        return 1;
    }

    return 0;
}

/* complete the function */
int isFull(QueueType *cQ)
{//Queue�� ���� á���� Ȯ���ϴ� �Լ�
    if(((cQ->rear+1) % MAX_QUEUE_SIZE) == cQ->front)	//ť�� ���� á����
    {
		printf("Already Queue is Empty.\n");	//��� ���
        return 1;
    }

   return 0;
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{//Queue�� element�� ���� �ϴ� �Լ�
    if(isFull(cQ)==1)	//ť�� ���� á����
    {
        return;
    }

	else	//ť�� ���� ������ ������
	{//ť ����
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		cQ->queue[cQ->rear] = item;
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{//Queue�� element�� ���� �ϴ� �Լ�
    if(isEmpty(cQ)==1)	//ť�� ���������
    {
        return;
    }

	else	//ť�� ������� ������
	{//ť ����
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;
		*item = cQ->queue[cQ->front];	//ť�� �����δ� ���������� ������·� ��
	}
}


void printQ(QueueType *cQ)
{//Queue�� ��� �ϴ� �Լ�
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first�� ���� �ε��� ��ġ 
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	//last�� ���� �ε��� ��ġ

	printf("Circular Queue : [");

	//first���� last���� ť ���
	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);	
		i = (i+1)%MAX_QUEUE_SIZE;		

	}
	
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{//Queue�� ������ �ִ��� Ȯ���ϴ� �Լ�

	//Queue�� ������ �������� ó�� ���
	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) 		
		{
			printf("  [%d] = front\n", i);	//front�� ��ġ ���
			continue;
		}

		else if(i > cQ->front)	
		{
			printf("  [%d] = %c\n", i, cQ->queue[i]);
			continue;
		}

		else					
		{
			printf("  [%d] = \n", i);
			continue;
		}
	}

	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	//front�� rear�� �ε��� ��ġ ���
}

