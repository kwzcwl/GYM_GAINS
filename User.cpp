#include "User.h"
#include "Menus.h"
#include <iostream>

using namespace std;




User::User(string n,string sn,string passwd,string un)
{
	name = n;
	surname = sn;
	password = passwd;
	if(un == " ")
	{
		username = un;
	}
	else
	{
		username = n;
	}
}

void User::addWorkout(User& user)
{
	cout<<endl<<"Adding Workout, enter workout name:\t";
	string wname;
	cin.ignore();
	getline(cin,wname);

	workouts.push_back(Workout(wname,"12.05.2025"));

	Workout& w = workouts.back();
	
	workout_menu(w,user);
}
