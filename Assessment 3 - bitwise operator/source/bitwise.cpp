#include "bitwise.h"

using namespace std;

void Bitwise::convertToBinary(int a_decInput)
{
	char num = a_decInput;
	//char result = 0;

	for(int i = 7; i >= 0; i--)
	{
		if(((num >> i) & 1) == 1)
			cout << "1";			
		else
			cout << "0";			
	}		
}

char Bitwise::convertToInt(std::string a_binInput)
{	
	char num = 0;

	char result = 0;

	//make sure we have a full string to work with	
	if(a_binInput.length() < 8)
	{
		a_binInput.insert(0, (8 - a_binInput.length()),'0');
	}
	
	for(int i = 0; i < 8; i++)
	{
		int index = 7 - i;

		int val = (int)pow(2, index);
		
		if(a_binInput[i] == '1')
		{
			num+= val;
		}			
	}

	return num;
}