#include "Inputs.h"
#include <conio.h>

template <>
int stringToType<int>(const std::string& str)
{
    return std::stoi(str);
}

template <>
float stringToType<float>(const std::string& str)
{
    return std::stof(str);
}

template <>
double stringToType<double>(const std::string& str)
{
    return std::stod(str);
}

template <>
std::string getSafeInput<std::string>(std::string prompt)
{
    std::string userInput;
    std::cout << prompt;
    std::getline(std::cin, userInput);
    return userInput;
}



std::string getHiddenInput(std::string prompt)
{
	std::cout<<std::endl<<prompt;

	std::string input = "";
	char ch = ' ';

	while ((ch = _getch()) != '\r')
	{
        if (ch == '\b')
		{
            if (!input.empty())
			{
                input.pop_back();
                std::cout << "\b \b"; 
            }
        } 
		else
		{
            input += ch;
            std::cout << '*';
        }
    }
    std::cout<<std::endl;

    return input;
}