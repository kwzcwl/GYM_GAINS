#include "Workout.h"
#include "User.h"
#include "Menus.h"
#include "Inputs.h"
#include <fstream>

using namespace std;


Workout::Workout(string n,string d)
{
	name = n;
	date = d;
}

Workout::~Workout()
{
	for (auto& e : excercises)
    {
        delete e.second;
    }
    excercises.clear();
}

void Workout::save(ofstream& ofs)
{
	ofs << name << "\n" << date << "\n";
    ofs << excercises.size() << "\n";
    for (const auto& e : excercises)
	{
        ofs << e.first << "\n";
        e.second->save(ofs);
    }
}

void Workout::addExcercise(Workout& w, User& u)
{
	int id = 1;
	
	for(auto& ex : excercises)
	{
		if(ex.first==id)
		{
			id++;
		}
	}
	
	cout<<endl<<"Adding "<<id<<" excersise"<<endl;
	cout<<"-----------------------"<<endl;

	string name = getSafeInput<string>("Enter the name of excercise: ");
	
	excercises[id] = new Excercise(name);
}

void Workout::printWorkout()
{
	cout<<"\033[2J\033[1;1H";
	cout<<"============================================"<<endl;
	cout<<date<<"\t"<<name<<endl;
	cout<<"============================================"<<endl;
	for(auto& ex : excercises)
	{
		ex.second->printExcercise();
	}
}

void Workout::addSet(Workout& w, User& u)
{
	for(auto& ex:excercises)
	{
		cout<<ex.first<<"\t-\t"<<ex.second->name<<endl;
	}

	int input = getSafeInput<int>("For which excercise do you want to add the set?\t");

	for(auto& ex:excercises)
	{
		if(ex.first == input)
		{
			ex.second->addSet();
			break;
		}
	}
}
