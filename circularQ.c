/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */
#include <stdio.h>
#include <stdlib.h>
//<stdio.h>와 <stdlib.h>가 컴파일 전에 소스에 확장하도록 지시

#define MAX_QUEUE_SIZE 4    
//MAX_QUEUE_SIZE를 상수로 지정 후 4 저장 

typedef char element;
//element 구조체 선언

typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;    //front와 rear 선언
}QueueType;
//QueueType 구조체 선언

//함수 원형
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
    //QueueType 객체 *cQ 선언 후 큐 생성
	QueueType *cQ = createQueue();
	element data;   //element 객체 data 선언
	char command;   //사용자로부터 명령을 입력받을 변수 command 선언
	
	printf("[----- [고영민] [2019038003] -----]");

    //q or Q가 입력 될때까지 반복하는 반복문
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	//사용자로부터 Command를 입력받음

		switch(command){
		case 'i': case 'I':		//Command가 i or I 일때
			data = getElement();	
			enQueue(cQ, data);		
			break;

		case 'd': case 'D':		//Command가 d or D 일때
			deQueue(cQ, &data);
			break;

		case 'p': case 'P': 	//Command가 p or P 일때
			printQ(cQ);
			break;

		case 'b': case 'B':		//Command가 b or B 일때
			debugQ(cQ);
			break;

		case 'q': case 'Q':		//Command가 q or Q 일때
   	        freeQueue(cQ);
			break;

		default:				//Command를 잘못 입력 했을때
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;   //1 반환
}

QueueType *createQueue()
{//Queue를 생성하는 함수
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;	//front값 0으로 초기화
	cQ->rear = 0;	//rear값 0으로 초기화
	return cQ;
}

//rear와 front가 같을때 empty rear와 front가 가장 멀때 full 
 
int freeQueue(QueueType *cQ)
{//Queue의 메모리 할당을 free해주는 함수
    if(cQ == NULL) return 1;    //cQ가 비어있을때 1 반환
    free(cQ);   //cQ가 비어있지 않을때 free
    return 1;   //1 반환
}

element getElement()
{//사용자로부터 element를 받는 함수
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{//Queue가 비어 있는지 확인하는 함수
    if((cQ->front) == (cQ->rear))	//큐가 비어있을때
    {
		printf("Already Queue is Full.\n");		//경고문 출력
        return 1;
    }

    return 0;
}

/* complete the function */
int isFull(QueueType *cQ)
{//Queue가 가득 찼는지 확인하는 함수
    if(((cQ->rear+1) % MAX_QUEUE_SIZE) == cQ->front)	//큐가 가득 찼을때
    {
		printf("Already Queue is Empty.\n");	//경고문 출력
        return 1;
    }

   return 0;
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{//Queue에 element를 삽입 하는 함수
    if(isFull(cQ)==1)	//큐가 가득 찼을때
    {
        return;
    }

	else	//큐가 가득 차있지 않을때
	{//큐 삽입
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
		cQ->queue[cQ->rear] = item;
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{//Queue에 element를 삭제 하는 함수
    if(isEmpty(cQ)==1)	//큐가 비어있을때
    {
        return;
    }

	else	//큐가 비어있지 않을때
	{//큐 삭제
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;
		*item = cQ->queue[cQ->front];	//큐가 실제로는 남아있지만 공백상태로 봄
	}
}


void printQ(QueueType *cQ)
{//Queue를 출력 하는 함수
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first의 현재 인덱스 위치 
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	//last의 현재 인덱스 위치

	printf("Circular Queue : [");

	//first부터 last까지 큐 출력
	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);	
		i = (i+1)%MAX_QUEUE_SIZE;		

	}
	
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{//Queue에 문제가 있는지 확인하는 함수

	//Queue가 실제로 지워진것 처럼 출력
	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) 		
		{
			printf("  [%d] = front\n", i);	//front의 위치 출력
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

	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	//front와 rear의 인덱스 위치 출력
}

