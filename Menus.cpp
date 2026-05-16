#include "Menus.h"
#include "User.h"
#include "Workout.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include "Inputs.h"

using namespace std;

vector<User> users;

User* signin()
{
	string inUsername, inPasswd;
    cout << "Username: ";
    getline(cin, inUsername);
    
    inPasswd = getHiddenInput("Password: ");

    for (auto& u : users)
	{
        if (u.username == inUsername && u.checkPassword(inPasswd))
		{
            return &u;
        }
    }

    return nullptr;
}




void workout_menu(Workout& w,User& user)
{
	w.printWorkout();

	switch(getSafeInput<int>("[0] - Add Excercise\t[1] - Add Set\t[2] - Finish Workout\t:\t"))
	{
		case 0:
			w.addExcercise(w,user);
			break;

		case 1:
			w.addSet(w,user);
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
	switch(getSafeInput<int>("[0] - Add Workout\t[1] - Check workout history\t[2] - Check Progress\t[3] - Sign out\t:\t"))
	{
		case 0:
			user.addWorkout(user);
			break;

		case 1:
			user.displayWorkouts();
			user_menu(user);
			break;

		case 2:
			cout<<"not yet implemented";
			break;

		case 3:
			main_menu();

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

	switch(getSafeInput<int>("[0] - Sign in\t[1] - Sign up\t[2] - Exit app\t:\t"))
	{
		case 0:
		{
			User* user = signin();

			if (user != nullptr)
			{
				cout << "Login successful! Welcome " << user->name << endl;
				user_menu(*user); 
			}
			else
			{
				cout << "Invalid username or password." << endl;
				main_menu();
			}
			break;
		}
		case 1:
		{
			User new_user = signup();
			users.push_back(new_user);
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