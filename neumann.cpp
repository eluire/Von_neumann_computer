#include <iostream>
#include <assert.h>
#include <typeinfo>
#include <string>
#include <stdlib.h>

using namespace std;

void read(string operand, int mainStack[]);
void write(string operand, int mainStack[]);
void load(string operand, int mainStack[], int &accumulator);
void loadMQ(int &accumulator);
void loadMQ_adress(string operand, int mainStack[] ,int &MQ);
void store(string operand, int mainStack[], int accumulator);
void add(string operand, int mainStack[], int &accumulator);
void subtract(string operand, int mainStack[], int &accumulator);
void divide(string operand, int mainStack[], int &accumulator ,int &MQ);
void multiply(string operand, int mainStack[], int &accumulator ,int &MQ);
int bd(string hadenia);

int prompt();
void dump();

//Pilha principal com todos os dados - Simula a memória
int mainStack[1024];

// Registradores
int accumulator = 0;
int MQ = 0;

string MBR,  MAR, PC, IR;


int main()
{
    cout 		<< ">>>    	Bem vindo a Neumann \n"
                << ">>> 	Digite uma instrucao ou digite 'cancel' para sair\n"<<endl;

    prompt();
    /**********
    TESTAR OS BAGULHO
    **********/
    mainStack[600] = 37;  //1001011000
    mainStack[601] = 80; //1001011001
    mainStack[602] = 23; //1001011010
    mainStack[603] = 16; //1001011011
    mainStack[604] = 63; //1001011100
    mainStack[605] = 11; //1001011101
//606 - 1001011110
//607 - 1001011111
//608 - 1001100000
//609 - 1001100001

//LOAD MQ, 602 -
    loadMQ_adress("1001011010",mainStack, MQ);
//MUL 601
    multiply("1001011001",mainStack,accumulator, MQ);
//LOAD MQ
    loadMQ(MQ);
//ADD 600
    add("1001011000",mainStack, accumulator);
//STOR 607
    store("1001011111",mainStack, accumulator);
//LOAD MQ, 604
    loadMQ_adress("1001011100",mainStack, MQ);
//MUL 605
    multiply("1001011101",mainStack, accumulator, MQ);
//LOAD MQ
    loadMQ(MQ);
//STOR 608
    store("1001100000",mainStack, accumulator);
//LOAD 603
    load("1001011011",mainStack, accumulator);
//SUB 608
    subtract("1001100000",mainStack, accumulator);
//STOR 609
    store("1001100001",mainStack, accumulator);
//LOAD 607
    load("1001011111",mainStack, accumulator);
//DIV 609
    divide("1001100001",mainStack, accumulator, MQ);
//STOR 607
    store("1001011111",mainStack, accumulator);
//LOAD MQ
    loadMQ(MQ);
//STOR 606
    store("1001011110",mainStack, accumulator);

    cout<<endl;
    cout<<"MQ: "<<MQ<<" "<<endl;
    cout<<"AC: "<<accumulator<<endl;
    cout<<"MEMORIA "<<endl;
    cout<<"POSICAO | CONTEUDO "<<endl;
    for(int i=600; i<610; i++)
    {
        cout<<"| "<<i<<" | "<<mainStack[i]<<endl;
    }
    cout<<"RESULTADO DA FUNCAO: "<<mainStack[606]<<endl;

    return 0;
}

/*
* Shows the user a prompt where they can enter
* instructions that they want to execute.
* This function simply calls other functions
* based on the operation
*/
int prompt()
{
    string currentInstruction = ""; //Instrução atual
    string operation = ""; // OPcode
    string operand = ""; //endereço dont care
    int currentLine = 0;


    // Armazena a instrução na memória

    do
    {

        cout << "[" << currentLine << "]: "; //Mostra a linha atual

        cin >> currentInstruction;
        //A instrução de 16bits é dividida entre os 6 primeiros para o OPcode
        //e os ultimos 10 o endereco da memória que será ultilizado na atual instrução
        if(currentInstruction == "cancel")
        {
            return 1;
        }
        //Verifica se é um valor valido
        while (currentInstruction.length() != 16)
        {
            cout << "Digite um valor valido" << '\n';
            cin >> currentInstruction;
            if(currentInstruction == "cancel")
            {
                return 1;
            }
        }
        //CICLO DE BUSCA
        PC = currentInstruction;
        MBR = PC;

        operation 	= MBR.substr(0,6); //Opcode
        operand 	= MBR.substr(6,16); //Endereço da memória
        currentLine++;

        IR = operation;  //Opcode
        MAR = operand;     //Endereço da memória

        //Verifica qual instrução foi digitada

        if(IR == "111111")
        {
            read(MAR, mainStack);
        }
        else if(IR =="000000")
        {
            write(MAR, mainStack);
        }
        else if(IR =="000001")
        {
            load(MAR, mainStack, accumulator);
        }
        else if(IR =="001010" )
        {
            loadMQ(accumulator);
        }
        else if(IR =="001001")
        {
            loadMQ_adress(MAR, mainStack ,MQ);
        }
        else if(IR =="100001")
        {
            store(MAR, mainStack, accumulator);
        }
        else if(IR =="000101")
        {
            add(MAR, mainStack, accumulator);
        }
        else if(IR =="000110")
        {
            subtract(MAR, mainStack, accumulator);
        }
        else if(IR =="001100")
        {
            divide(MAR, mainStack, accumulator,MQ);
        }
        else if(IR =="001011")
        {
            multiply(MAR, mainStack, accumulator,MQ);
        }
        else
        {
            cout << "Digite um valor valido!";
            break;
        }
    }
    while (currentInstruction.length() == 16);
    return 0;
}


/********************************************************
* DECLARAÇÃO DAS FUNÇÕES!
*******************************************************/

//Aloca os valores na memória
void read(string operand, int mainStack[]){
    int address = bd(operand); 	// Converte o address de string para inteiro
    int userInput = 0;
    cout<<"Digite o valor"<<endl;
    cin >> userInput;  //Valor no endereço digitado
    mainStack[address] = userInput;
}
//Mostra Valores no endereço digitado
void write(string operand, int mainStack[]){
    int address = bd(operand);
    cout << mainStack[address]<<endl;
}
void load(string operand, int mainStack[], int &accumulator)
{
    int address = bd(operand);
    accumulator = mainStack[address];
    cout<<"LOAD "<<address<<endl;
}

void store(string operand, int mainStack[], int accumulator)
{
    int address = bd(operand);
    mainStack[address] = accumulator;
    cout<<"STOR "<<address<<endl;
}

void add(string operand, int mainStack[], int &accumulator)
{
    int address = bd(operand);
    int soma = mainStack[address] + accumulator;
    accumulator = soma;
    cout<<"ADD "<<address<<endl;
}

void subtract(string operand, int mainStack[], int &accumulator)
{
    int address = bd(operand);
    int value = accumulator - mainStack[address];
    accumulator = value;

    cout<<"SUB "<<address<<endl;
}

void divide(string operand, int mainStack[], int &accumulator ,int &MQ)
{

    int address = bd(operand);
    int value = accumulator / mainStack[address];
    int resto = accumulator % mainStack[address];
    MQ = resto;
    accumulator = value;
    cout<<"DIV "<<address<<endl;
}
//fazer função pra store mq->ac
void multiply(string operand, int mainStack[], int &accumulator , int &MQ)
{
    int address = bd(operand);
    int value = MQ * mainStack[address];
    accumulator = value;
    cout<<"MULT "<<address<<endl;
}
void loadMQ(int &MQ)
{
    MQ = accumulator;
    cout<<"LOAD MQ "<<endl;
}
void loadMQ_adress(string operand, int mainStack[], int &MQ)
{
    int address = bd(operand);
    MQ = mainStack[address];
    cout<<"LOAD MQ, "<<address<<endl;

}

//Transformar string para inteiro decimal
int bd(string infor)
{
    int num = atoi(infor.c_str());
    int rem, temp, dec = 0, b = 1;
    temp = num;
    while (temp > 0)
    {
        rem = temp % 10;
        dec = dec + rem * b;
        b *= 2;
        temp /= 10;
    }

    return dec;
}
