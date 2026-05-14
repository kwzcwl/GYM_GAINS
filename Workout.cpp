#include "Workout.h"
#include "User.h"
#include "Menus.h"

using namespace std;


Workout::Workout(string n,string d)
{
	name = n;
	date = d;
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
	

	string name;
	
	cout<<endl<<"Adding "<<id<<" excersise"<<endl;
	cout<<"-----------------------"<<endl;
	cout<<"Enter the name of excercise: ";
	cin.ignore();
	getline(cin,name);
	
	excercises.insert({id,Excercise(name)});

	workout_menu(w,u);
}

void Workout::printWorkout()
{
	cout<<"\033[2J\033[1;1H";
	cout<<"============================================"<<endl;
	cout<<date<<"\t"<<name<<endl;
	cout<<"============================================"<<endl;
	for(auto& ex : excercises)
	{
		ex.second.printExcercise();
	}
}

void Workout::addSet(Workout& w, User& u)
{
	cout<<endl<<"For which excercise do you want to add the set?"<<endl;
	for(auto& ex:excercises)
	{
		cout<<ex.first<<"\t-\t"<<ex.second.name<<endl;
	}

	int input;

	cin>>input;

	for(auto& ex:excercises)
	{
		if(ex.first == input)
		{
			ex.second.addSet();
			workout_menu(w,u);
			break;
		}
	}
}
