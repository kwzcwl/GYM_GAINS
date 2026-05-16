#include "User.h"
#include "Menus.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <ranges>
#include "Inputs.h"
#include <fstream>

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
	username = un;
}

User::~User()
{
	for(auto& w : workouts)
	{
		delete w;
	}
	workouts.clear();
}

void User::save(ofstream& ofs)
{
	ofs << username << "\n" << name << "\n" << surname << "\n" << password << "\n";
    ofs << workouts.size() << "\n";
    for (Workout* w : workouts)
	{
        w->save(ofs);
    }
}

User* User::load(std::ifstream& ifs)
{
    std::string un, n, sn, pw;
    if (!std::getline(ifs, un) || !std::getline(ifs, n) || !std::getline(ifs, sn) || !std::getline(ifs, pw))
	{
        return nullptr;
    }

    User* newUser = new User(n, sn, pw, un);
    
    size_t workoutCount = 0;
    ifs >> workoutCount;
    ifs.ignore();

    for (size_t i = 0; i < workoutCount; ++i)
	{
        Workout* loadedWorkout = Workout::load(ifs);
        if (loadedWorkout != nullptr)
		{
            newUser->workouts.push_back(loadedWorkout);
        }
    }

    return newUser;
}

void User::addWorkout(Database& db)
{
	string wname = getSafeInput<string>("Adding Workout, enter workout name:\t");

	workouts.push_back(new Workout(wname,getDate()));

	Workout& w = *workouts.back();
	
	workout_menu(w,*this,db);
}

void User::displayWorkouts()
{
	cout<<endl<<"\033[2J\033[1;1H";

	for (size_t i = 0; i < workouts.size(); i++)
	{
		cout<<i<<"\t-\t"<<workouts[i]->name<<"\t"<<workouts[i]->date<<endl;
	}
}

bool User::checkPassword(string inPasswd)
{
	return inPasswd == password;
}