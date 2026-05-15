#include "Menus.h"
#include "User.h"
#include "Workout.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <conio.h>

using namespace std;

string getHiddenInput(string prompt)
{
	cout<<endl<<prompt;

	string input = "";
	char ch = ' ';

	while ((ch = _getch()) != '\r')
	{
        if (ch == '\b')
		{
            if (!input.empty())
			{
                input.pop_back();
                cout << "\b \b"; 
            }
        } 
		else
		{
            input += ch;
            cout << '*';
        }
    }
    cout<<endl;

    return input;

}

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

	switch(getSafeIntInput("[0] - Add Excercise\t[1] - Add Set\t[2] - Finish Workout\t:\t"))
	{
		case 0:
			w.addExcercise(w,user);		//
			break;

		case 1:
			w.addSet(w,user);		//
			break;

		case 2:
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
	switch(getSafeIntInput("[0] - Back to main menu\t[1] - Add Workout\t[2] - Check old workouts\t:\t"))
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

User signup()
{
	cout<<endl<<"Name:\t";
	string name;
	getline(cin,name);

	cout<<endl<<"Surname:\t";
	string surname;
	getline(cin,surname);

	cout<<endl<<"Username:\t";
	string username;
	getline(cin,username);

	string password1 = getHiddenInput("Password:\t");
	string password2 = getHiddenInput("Confirm password:\t");

	while (password1 != password2)
	{
		cout<<endl<<"Passwords do not match!";

		string password1 = getHiddenInput("Password:\t");
		string password2 = getHiddenInput("Confirm password:\t");
	}

	cout<<endl<<"User succesfully added!"<<endl<<endl;

	User user(name,surname,password1,username);

	return user;
}

void main_menu()
{

	switch(getSafeIntInput("[0] - Sign in\t[1] - Sign up\t[2] - Exit app\t:\t"))
	{
		case 0:
			//sign in menu
			break;

		case 1:
		{
			User new_user = signup();
			user_menu(new_user);
			break;
		}
		case 2:
			exit_program();
			break;

		default:
			main_menu();
			break;
	}
}