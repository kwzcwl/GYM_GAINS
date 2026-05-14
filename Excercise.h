#pragma once
#include <string>
#include <map>
#include "Set.h"

using namespace std;

struct Excercise
{
    public:
		string name;
        map<int,Set> sets;

    	Excercise(string n);

		void addSet();
		void printExcercise();
};