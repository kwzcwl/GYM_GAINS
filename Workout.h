#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Excercise.h"

struct User;

using namespace std;

struct Workout
{
    public:

		map<int,Excercise> excercises;
        string name;
        string date;
        string duration;

    	Workout(string n,string d);
		
		void addExcercise(Workout& w, User& u);
		void printWorkout();
		void addSet(Workout& w, User& u);
};