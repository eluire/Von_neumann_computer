#include <iostream>
#include <assert.h>
#include <typeinfo>
#include <string>


using namespace std;
/**********************************************
Function prototypes
*********************************************/
// Shows the main prompt
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


// This is the main stack that contains all the data
int mainStack[1024];

// This is the A register in the CPU
int accumulator = 0;

int MQ = 0;



int main() {
	cout 		<< ">>>    	Welcome to Neumann \n"
				<< ">>> 	Please enter you program below after the ?\n"
				<< ">>> 	You can either enter an instruction\n"
				<< ">>> 	one a time or enter a data byte.\n";

	// DONE: Write a function that displays the prompt
	// DONE: Implement the master array!
	// TODO: Write out the Program Design Document
	// TODO: Add a dump function that dumps the contetnts of the array


	prompt();
	return 0;
}

/*
* Shows the user a prompt where they can enter
* instructions that they want to execute.
* This function simply calls other functions
* based on the operation
*/
int prompt() {
	string currentInstruction = "";
	string operation = ""; // OPcode
	string operand = ""; //endereço dont care
	int currentLine = 0;
	int operationNumeric;		// int conversion of operation for switch statement



	// Stores each instruction in Neumann's memory
	do{
		// 9999 is the command to quit I
		// Displays the prommpt like this "[0]:"
		cout << "[" << currentLine << "]: ";

		cin >> currentInstruction;

		//mainStack[currentLine] = stoi(currentInstruction);

		// Split the first two letters and assign that to operation key
		// Split the remaining two and assign that to operand
		if(currentInstruction == "cancel"){
			return 1;
		}
		while (currentInstruction.length() != 16) {
			cout << "digite um valor valido" << '\n';
			cin >> currentInstruction;
			if(currentInstruction == "cancel"){
				return 1;
			}
		}

		operation 	= currentInstruction.substr(0,6);
		operand 	= currentInstruction.substr(6,16);
		currentLine++;


	/**************************************************************************
	***************************************************************************
	Updates: Added the functions to load and add string to the memory.
	Updates: Need to write out the design of the program.
	***************************************************************************
	***************************************************************************/

	// User pressed 9999 thus is done writing instructions!
	// Time to execute now!

		if(operation == "111111"){
			read(operand, mainStack);
		}

		else if(operation =="000000"){
			write(operand, mainStack);
		}
		else if(operation =="000001"){
			load(operand, mainStack, accumulator);
		}
		else if(operation =="001010" ){
			loadMQ(accumulator);
		}
		else if(operation =="001001"){
			loadMQ_adress(operand, mainStack ,MQ);
		}
		else if(operation =="100001"){
			store(operand, mainStack, accumulator);
		}
		else if(operation =="000101"){
			add(operand, mainStack, accumulator);
		}
		else if(operation =="000110"){
			subtract(operand, mainStack, accumulator);
		}
		else if(operation =="001100"){
			divide(operand, mainStack, accumulator,MQ);
		}
		else if(operation =="001011"){
			multiply(operand, mainStack, accumulator,MQ);
		}
		else{
			cout << "Digite um valor valido!";
			break;
		}
	}
	while (currentInstruction.length() == 16);
	return 0;
}



/********************************************************
* FUNCTION DECLARATIONS!
*******************************************************/
void read(string operand, int mainStack[]){

	int address = bd(operand); 	// convert the address from string to integer
	int userInput = 0;
	cin >> userInput;
	mainStack[address] = userInput;
}

void write(string operand, int mainStack[]){
	int address = bd(operand);	// convert the address from string to integer
	cout << mainStack[address];
}
void load(string operand, int mainStack[], int &accumulator){
	// Load takes the given address and stores the data in that location
	// in the memory to the A register
	int address = bd(operand); 	// convert the address from string to integer
	accumulator = mainStack[address];
	cout << "Transfere o conteudo " << address << " da memoria para o acumulador [" << accumulator<< "]\n";
}

void store(string operand, int mainStack[], int accumulator){
	// Store a word from the memory into the accumulator
	int address = bd(operand);
	mainStack[address] = accumulator;
	cout << "Transfere o conteudo " << accumulator << " do acumulador para a memoria no endereco" << address << "[" << mainStack[address] << "]\n";

}

void add(string operand, int mainStack[], int &accumulator){
	// Add the value in the given address into the accumulator
	// and leave the result there
	int address = bd(operand);
	int soma = mainStack[address] + accumulator;
	accumulator = soma;
	cout << "Soma o conteudo da memoria no endereco " << address << " com o acumulador e grava no acumulador = " << accumulator << "\n";

}

void subtract(string operand, int mainStack[], int &accumulator){
	// Subtract the value in the given address into the accumulator
	// and leave the result there
	int address = bd(operand);
	int value = accumulator - mainStack[address];
	accumulator = value;

	cout << "subtrai o conteudo do acumulador com o conteudo da memoria no do endereco " << address<<" e grava no acumulador = " << accumulator << "\n";

}

void divide(string operand, int mainStack[], int &accumulator ,int &MQ){

	int address = bd(operand);
	int value = accumulator / mainStack[address];
	int resto = accumulator % mainStack[address];
	//accumulator = value_AC;
	MQ = resto;
	accumulator = value;
	cout << "divide o conteudo do acumulador com o conteudo da memoria no do endereco " << address <<" e grava o quociente no acumulador [" << accumulator <<"] e o resto da divisão no registrador MQ ["<< MQ <<"]"<<"\n";
}
//fazer função pra store mq->ac
void multiply(string operand, int mainStack[], int &accumulator , int &MQ){
	int address = bd(operand);
	int value = MQ * mainStack[address];
	accumulator = value;

	cout << "multiplica o conteudo do registrador MQ ["<<MQ<<"] com o conteudo da memoria no do endereco " << address <<" e grava os bits mais significativos no acumulador = " << accumulator << "e os menos significativos no MQ"<<"\n";


}
void loadMQ(int &MQ)
{
	MQ = accumulator;
	cout << "Transfere o conteudo do acumulador "<<accumulator <<" para o registrador MQ ["<<MQ<<"]"<<'\n';

}
void loadMQ_adress(string operand, int mainStack[], int &MQ){
	int address = bd(operand);
	MQ = mainStack[address];
	cout << "Transfere o conteudo "<<mainStack[address]<<" da memoria para o registrador MQ ["<< MQ <<"]"<<'\n';
}
int bd(string hadenia)
{
		int num = atoi(hadenia.c_str());
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
