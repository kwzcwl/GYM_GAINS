#pragma once
#include <string>
#include <stdexcept>
#include <iostream>


template <typename T>
T stringToType(const std::string& str);

template <> int stringToType<int>(const std::string& str);
template <> float stringToType<float>(const std::string& str);
template <> double stringToType<double>(const std::string& str);

//apparently due to it being a template function it has to be defined in the header file not in .cpp
template <typename T>
T getSafeInput(std::string prompt) 
{
    std::string userInput;

    while(true)
    {
        std::cout << prompt;
        std::getline(std::cin, userInput);

        try 
        {
            return stringToType<T>(userInput);
        }
        catch (const std::invalid_argument& e) 
        {
            std::cout<<std::endl<<"\t!!! INVALID INPUT !!!\n";
        }
        catch (const std::out_of_range& e) 
        {
            std::cout<<std::endl<<"\t!!! INVALID INPUT !!!\n";
        }
    }
}

template <> 
std::string getSafeInput<std::string>(std::string prompt);

std::string getHiddenInput(std::string prompt);