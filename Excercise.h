#pragma once
#include <string>
#include <map>
#include "Set.h"

using namespace std;

struct Excercise
{
    public:
		string name;
        map<int,Set*> sets;

    	Excercise(string n);

		~Excercise();
		Excercise(const Excercise& other);
    	Excercise& operator=(const Excercise& other);

		void save(ofstream& ofs);
		//static Excercise* load(ifstream& ifs);

		void addSet();
		void printExcercise();
};