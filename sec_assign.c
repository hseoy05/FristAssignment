#include <stdio.h>
#include <string.h>
#define STACK_SIZE 50

int     call_stack[STACK_SIZE];
char    stack_info[STACK_SIZE][20];

int SP = -1; 
int FP = -1;

void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);


//------------------push, pop 함수 ---------------------
void push(int memory_data, char* about_info) {
    SP += 1;
    call_stack[SP] = memory_data;
    strcpy(stack_info[SP], about_info);
}

int pop() {
    int return_val = call_stack[SP];
    call_stack[SP] = 0;
    strcpy(stack_info[SP], "0");
    SP -= 1;
    return return_val;
}
//-------------------------------------------------



//-----------prolog, epillog 함수 --------------------
void prolog(char* func_name, int local_num){
    //1. return adress
    push(-1, "Return Adress");

    //2. SFP 
    char buf[50];
    sprintf(buf, "%s SFP", func_name); 
    push(FP, buf);
    FP = SP;

    //3. local num +
    SP += local_num;
}

void epillog(){
    FP = pop(); //SFP에서 이전 함수의 FP값 불러오기기
    int nul_val = pop(); //return adress 값 지우기기
}
//-------------------------------------




void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");
    
    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i ,stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}


//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;

    //prolog start
    prolog("func1", 1);
        //local value 저장
    SP -= 1;
    push(100, "var_1");
    //prolog end
    print_stack();

    //다음 호출될 함수의 매개변수 저장장
    push(13, "arg2");
    push(11, "arg1");
    func2(11, 13);

    //epillog start
    SP = FP;
    epillog();
         //호출한 함수에서 매개변수 정리하기기
    pop();
    pop();
    //epillog end

    print_stack();
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;
    //prolog start
    prolog("func2", 1);
        //local value 저장
    SP -= 1;
    push(200, "var_2");
    //prolog end

    print_stack();

    //다음 호출 될 매개변수 저장
    push(77, "arg1");

    func3(77);
    //epillog start
    SP = FP;
    epillog();
    //i호출한 함수에서 매개변수 정리
    pop();
    //epillog end

    print_stack();
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;
    //prolog start
    prolog("func3", 2);
    //local value push
    SP -= 2;
    push(300, "var_3");
    push(400, "var_4");
    //prolog end
    print_stack();
}

int main()
{
    //0. 매개변수 push
    push(3, "arg3");
    push(2, "arg2");
    push(1, "arg1");

    func1(1, 2, 3);

    //epillog start
    SP = FP;
    epillog();
    //매개변수 정리리
    pop();
    pop();
    pop();
    //epillog end
    print_stack();
    return 0;
}