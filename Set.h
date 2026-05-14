#pragma once
#include <string>
#include <iostream>

using namespace std;

struct Set
{
    public:
		int reps;
        float weight;
        int RPE;
        string comment;

        Set(int r = 0, float w = 0, int R = 0, string c = "");
		void printSet();
};