/* postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//<stdio.h>,<stdlib.h>,<string.h>가 컴파일 전에 소스에 확장하도록 지시

#define MAX_STACK_SIZE 10	//MAX_STACK_SIZE를 10으로 초기화하고 상수로 지정
#define MAX_EXPRESSION_SIZE 20	//MAX_EXPRESSION_SIZE를 20으로 초기화하고 상수로 지정

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;
//구조체 precedence로 우선순위를 정함

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

//함수 원형
void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

//프로그램의 메인함수
int main()
{
	int count = 0;	//Infix전에 Postfix나 Eval을 할수 없도록 하는 count 선언
	char command;	//사용자로부터 입력받을 command 선언
	
	printf("[----- [고영민] [2019038003] -----]");

	//command가 q or Q일때 까지 반복하는 반복문
	do{	//메뉴 출력
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);	//사용자로부터 command를 입력받음

		switch(command) {
		case 'i': case 'I':	//command가 i or I 일때
			getInfix();
			count++;
			break;
		case 'p': case 'P':	//command가 p or P 일때
			if(count==0)	//infix가 입력되지 않았을때
			{
				printf("Please enter the Infix first!\n");
				continue;
			}
			toPostfix();
			break;
		case 'e': case 'E':	//command가 e or E 일때
			if(count==0)	//infix가 입력되지 않았을때
			{
				printf("Please enter the Infix first!\n");
				continue;
			}
			evaluation();
			break;
		case 'd': case 'D':	//command가 d or D 일때
			debug();
			break;
		case 'r': case 'R':	//command가 r or R 일때
			reset();
			count = 0;
			break;
		case 'q': case 'Q':	//command가 q or Q 일때
			break;
		default:			//command를 잘못 입력했을때
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;	//1 반환
}

void postfixPush(char x)
{//postfixStack의 Top에서 삽입 하는 함수
    postfixStack[++postfixStackTop] = x;	//++Top후 Top에서 삽입
}

char postfixPop()
{//postfixStack의 Top에서 삭제 하는 함수 
    char x;
    if(postfixStackTop == -1)	//Stack이 비었을때
        return '\0';
    else {	//Stack이 차있을때
    	x = postfixStack[postfixStackTop--];	//Top에서 삭제후 Top--
    }
    return x;	//x 반환
}

void evalPush(int x)	
{//계산 값 삽입하는 함수
    evalStack[++evalStackTop] = x;		//++Top후 Top에서 삽입
}

int evalPop()
{
    if(evalStackTop == -1)	//Stack이 비어있을때
        return -1;	//-1 반환
    else	//Stack이 차있을때
        return evalStack[evalStackTop--];	//Top에서 삭제 한뒤 값 반환하고 Top--
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{//사용자로부터 infix를 입력받는 함수
    printf("Type the expression >>> ");
    scanf("%s",infixExp);	//infixExp에 문자열 형식으로 저장받음
}

precedence getToken(char symbol)
{//우선순위를 반환해주는 함수
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{// ???
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{//문자를 postfixExp에 옮겨주는 함수
	if (postfixExp == '\0')	//postfix가 널값이 아닐때 
		strncpy(postfixExp, c, 1); 	//c를 postfixExp에 복사
	else	//postfix가 널값일때
		strncat(postfixExp, c, 1);	//널값을 postfixExp 끝에 이어붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{//infix를 postfix로 변환하는 함수
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;	//exp에 infixExp 복사
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')	//*exp가 널값일때까지 반복하는 whil문
	{
		if(getPriority(*exp) == operand)	//exp의 값이 피연산자일때 
		{
			x = *exp;	//x에 exp값 복사
			charCat(&x);	//postfixExp에 저장
		}

		else if(getPriority(*exp) == lparen) 	//exp가 ( 일때
		{
			postfixPush(*exp);	//postfixStack에 저장
		}

		else if(getPriority(*exp) == rparen)	//exp가 ) 일때
		{
			//( )사이에서 연산자를 피연산자 뒤로 출력하기 위한 while문
			while((x = postfixPop()) != '(') //exp가 ( 일때까지 역순으로 postfixExp에 저장 후 ( 도 저장하지 않고 버림 
			{
				charCat(&x);
			}
		}

		else	//exp가 다른 연산자 일때
		{
			//연산자 우선순위 비교를 위한 while문
			while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
			{//연산 순서에 맞게 연산 후 postfixExp에 순차적으로 저장
				x = postfixPop();
				charCat(&x);
			}
			postfixPush(*exp); //postfixStack에 저장
		}

		exp++;	//exp주소 값 ++
	}

	//나머지 값들 순차적으로 저장하기 위한 while문
    while(postfixStackTop != -1)	//postfixStack이 빌때까지
    {//x를 역순으로 postfixExp에 저장
    	x = postfixPop();
    	charCat(&x);
    }
}

//infixExp,postfixExp,eavl result,postfixStack 출력하는 함수
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
	{
		printf("%c  ", postfixStack[i]);
	}

	printf("\n");
}

//infixExp,postfixExp,postfixStack을 전부 0으로 초기화하는 함수
void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
	{
		postfixStack[i] = '\0';
	}
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

//산술식 계산 하는 함수
void evaluation()
{
	int opr1, opr2, i;	//피연산자 받을 변수 선언 및 for문을 위한 변수 선언

	int length = strlen(postfixExp);	//length에 postfixExp 크기 저장
	char symbol;	//postfixExp를 하나씩 읽을 변수 선언
	evalStackTop = -1;	//evalStackTop을 -1로 설정

	for(i = 0; i < length; i++)	//length크기만큼 반복하는 for문
	{
		symbol = postfixExp[i];	//symbol에 postfixExp[i] 저장
		if(getToken(symbol) == operand) //symbol이 피연산자 일때
		{
			evalPush(symbol - '0'); //evalStack에 symbol값 저장
		}

		else //symbol이 연산자일때
		{
			//앞에 저장한 피연산자 2개를 evalStack에서 가져옴
			opr2 = evalPop();
			opr1 = evalPop();

			switch(getToken(symbol))	//symbol 값에 따라 연산 실행 
			{
			case plus: evalPush(opr1 + opr2); break;	//symbol이 +일때 +연산 후 evalStack에 저장
			case minus: evalPush(opr1 - opr2); break;	//symbol이 -일때 -연산 후 evalStack에 저장
			case times: evalPush(opr1 * opr2); break;	//symbol이 *일때 *연산 후 evalStack에 저장
			case divide: evalPush(opr1 / opr2); break;	//symbol이 /일때 /연산 후 evalStack에 저장
			default: break;	//symbol이 다른 값일때
			}
		}
	}

	/*마지막에는 연산이 다 끝난 후이므로 남은 값은 evalResult.
	마지막 값을 evalResult에 초기화시켜줌*/
	evalResult = evalPop();
}

