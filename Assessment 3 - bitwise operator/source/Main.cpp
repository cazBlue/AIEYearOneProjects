#include <Windows.h>
#include "bitwise.h"
#include <iostream>
#include <sstream>

using namespace std;

void decOps();
void binaryOps();
std::string getBinInput();
int getIntInput();
int inputType();

enum
{
	INPUTTYPE_BINARY,
	INPUTTYPE_INTEGER
};

int main()
{
	std::string binInput;

	bool exit = false;//exit program
	
	while(!exit)
	{
		int opType = 0;
		//validate user input
		while(opType < 1 || opType > 2)
		{
			system("cls");						
			std::string input;		
			cout << "------------- Bitwise operator program ------------" << endl;
			cout << "--------------------- Main Menu ------------" << endl;					
			cout << "1 - Operations \n2 - Exit" << endl;					
			getline(cin,input);
			opType = atoi(input.c_str());;			
		}

		if(opType == 2)
			break;

		//jump to operation menu
		if(opType == 1)
			binaryOps();
		else
			decOps();
	}

	//system("Pause");
}

void binaryOps()
{
	Bitwise bitFunct;	
	int opType = 0;
	bool returnToMain = false;

	while(!returnToMain)
	{
		system("cls");
		opType = 0;
		cout << "---- Perform operation ----" << endl;
		cout << "What do you want to do?" << endl;		
		cout << "1  - convert between base 2/10" << endl;
		cout << "2  - addition/subtraction base 2/10" << endl;
		cout << "3  - Bitwise AND" << endl;
		cout << "4  - Bitwise OR" << endl;
		cout << "5  - Bitwise NOT" << endl;
		cout << "6  - Bitwise XOR" << endl;
		cout << "7  - Bitwise Left Shift" << endl;
		cout << "8  - Bitwise Right Shift" << endl;
		cout << "9  - Return to main menu" << endl;
		cout << "10 - Exit" << endl;
		
		while(opType < 1 || opType > 10)
		{
			std::string input;		
			getline(cin,input);
			opType = atoi(input.c_str());
		}
				
		int resInt = 0;
		int inp1 = 0, inp2 = 0;
		char resChar1, resChar2, resChar3;
		switch(opType)
		{
		case 1:			
			system("cls");
			cout << "------------Conversion----------" << endl;
			cout << "Choose type to convert" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Binary number" << endl;
				resInt = bitFunct.convertToInt(getBinInput());
				cout << "Int result is" << endl;
				cout << resInt << endl;
			}
			else
			{
				cout << "Enter Integer" << endl;
				resInt = getIntInput();
				cout << "Binary result is" << endl;
				bitFunct.convertToBinary((char)resInt);
				cout << endl;
			}
			
			system("Pause");
			break;
		case 2:
			system("cls");
			cout << "------------Addition----------" << endl;

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number Two as Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Two  as Integer" << endl;
				inp2 = getIntInput();
			}

			cout << "Binary result is" << endl;
			bitFunct.convertToBinary((inp1 + inp2));			
			cout << endl << "Integer result is\n" << (inp1 + inp2) << endl;
			
			system("Pause");
			break;
		case 3:
			system("cls");
			cout << "------------Bitwise AND----------" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number Two as Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Two  as Integer" << endl;
				inp2 = getIntInput();
			}

			resChar1 = inp1;
			resChar2 = inp2;
			resChar3 = resChar1 & resChar2;
			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			
			cout << endl;
			system("Pause");
			break;
		case 4:
			system("cls");
			cout << "------------Bitwise OR----------" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number Two as Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Two  as Integer" << endl;
				inp2 = getIntInput();
			}

			resChar1 = inp1;
			resChar2 = inp2;
			resChar3 = resChar1 | resChar2;
			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			
			cout << endl;
			system("Pause");

			break;
		case 5:
			system("cls");
			cout << "------------Bitwise NOT----------" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

		
			resChar1 = inp1;			
			resChar3 = ~ resChar1;
			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			
			cout << endl;
			system("Pause");

			break;
		case 6:
			system("cls");
			cout << "------------Bitwise XOR----------" << endl;
			cout << "Binary Number One" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number Two as Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Two  as Integer" << endl;
				inp2 = getIntInput();
			}

			resChar1 = inp1;
			resChar2 = inp2;
			resChar3 = resChar1 ^ resChar2;
			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			
			cout << endl;
			system("Pause");

			break;
		case 7:
			system("cls");
			cout << "------------Bitwise Left Shift----------" << endl;
			
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number to shift by as Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number to shift by as Integer" << endl;
				inp2 = getIntInput();
			}

			resChar1 = inp1;
			resChar2 = inp2;
			resChar3 = resChar1 << resChar2;
			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			
			cout << endl;
			system("Pause");

			break;
		case 8:
			system("cls");
			cout << "------------Bitwise Right Shift----------" << endl;
			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number one as Binary" << endl;
				inp1 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number one as Integer" << endl;
				inp1 = getIntInput();
			}

			if(inputType() == INPUTTYPE_BINARY)
			{
				cout << "Enter Number to shift by Binary" << endl;
				inp2 = bitFunct.convertToInt(getBinInput());
			}
			else
			{
				cout << "Enter Number to shift by Integer" << endl;
				inp2 = getIntInput();
			}
			
			
			resChar1 = inp1;
			resChar2 = inp2;
			resChar3 = resChar1 >> resChar2;

			cout << "Binary result is" << endl;
			bitFunct.convertToBinary(resChar3);
			cout << endl << "Integer result is\n" << (int)resChar3 << endl;
			

			cout << endl;
			system("Pause");

			break;
		case 9:
			returnToMain = true;
			break;
		case 10:
			exit(1);
			break;
		default:			
			returnToMain = true;
			break;
		}		
	}
}

void decOps()
{
	bool returnToMain = false;
	int opType = 0;


	while(!returnToMain)
	{
	system("cls");

		returnToMain = true;
	}
}

std::string getBinInput()
{
	std::string input = "";

	while(input == "")
	{		
		//cout << "enter binary number" << endl;			
		getline(cin,input);		
	}

	return input;
}

int getIntInput()
{
	int usrIn = 0;

	while(usrIn == 0)
	{		
		//cout << "enter binary number" << endl;			
		cin >> usrIn;		
	}

	return usrIn;
}

int inputType()
{
	int inpType = -1;

	while(inpType < 0 || inpType > 1)
	{		
		cout << "Choose input type \n " << INPUTTYPE_BINARY << " - Binary \n " << INPUTTYPE_INTEGER << " - Int" << endl;			

		cin >> inpType;		
	}

	return inpType;
}