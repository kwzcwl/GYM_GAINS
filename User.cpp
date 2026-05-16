#include "User.h"
#include "Menus.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <ranges>
#include "Inputs.h"

using namespace std;


string getDate()
{
	auto now = chrono::system_clock::now();

	time_t currentTime = chrono::system_clock::to_time_t(now);

	tm* localTime = localtime(&currentTime);

	stringstream ss;

	ss << put_time(localTime, "%d.%m.%Y");

	return ss.str();
}


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
	/*cout<<endl<<"Adding Workout, enter workout name:\t";
	string wname;
	getline(cin,wname);*/

	string wname = getSafeInput<string>("Adding Workout, enter workout name:\t");

	workouts.push_back(Workout(wname,getDate()));

	Workout& w = workouts.back();
	
	workout_menu(w,user);
}

void User::displayWorkouts()
{
	cout<<endl<<"\033[2J\033[1;1H";

	for (size_t i = 0; i < workouts.size(); i++)
	{
		cout<<i<<"\t-\t"<<workouts[i].name<<"\t"<<workouts[i].date<<endl;
	}
}

bool User::checkPassword(string inPasswd)
{
	return inPasswd == password;
}