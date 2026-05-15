#include "Menus.h"
#include "User.h"
#include "Workout.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int getSafeIntInput(string prompt)
{
	string userInput;
	cout<<prompt;

	getline(cin,userInput);

	try
	{
		return stoi(userInput);
	}
	catch (const invalid_argument& e)
	{
		return -1;
	}
	catch (const out_of_range& e)
	{
		return -2;
	}
}


void workout_menu(Workout& w,User& user)
{
	w.printWorkout();


	cout<<endl<<"1 - Add Excercise"<<endl;
	cout<<endl<<"2 - Add Set"<<endl;
	cout<<endl<<"3 - Finish Workout"<<endl;

	switch(getSafeIntInput("[1/2/3]:\t"))
	{
		case 1:
			w.addExcercise(w,user);		//
			break;

		case 2:
			w.addSet(w,user);		//
			break;

		case 3:
			user_menu(user);
			break;

		default:
			workout_menu(w,user);
			break;
	}

}

void exit_program()
{
	
}

void user_menu(User& user)
{
	cout<<endl<<"0 - Back to main menu"<<endl;
	cout<<endl<<"1 - Add Workout"<<endl;
	cout<<endl<<"2 - Check old workouts"<<endl;


	switch(getSafeIntInput("[0/1/2]:\t"))
	{
		case 0:
			main_menu();
			break;

		case 1:
			user.addWorkout(user);
			break;

		case 2:
			user.displayWorkouts();
			user_menu(user);
			break;

		default:
			user_menu(user);
			break;
	}
}

void main_menu()
{
	cout<<endl<<"[M]-Menu\t[X]-Exit\t";
	string user_input;
	getline(cin,user_input);
	if(user_input=="M")
	{
		//user_menu();
	}
	else
	{
		if(user_input=="X")
		{
			exit_program();
		}
		else
		{
			main_menu();
		}
	}
}