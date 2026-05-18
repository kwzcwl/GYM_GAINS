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
#include <algorithm>

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

void User::analyzeProgress()
{
    cout << "\033[2J\033[1;1H";
    cout << "========= PROGRESS ANALYSIS =========" << endl;

    if (workouts.empty())
    {
        cout << "No workouts found to analyze." << endl;
        return;
    }

    
    bool hasLongWorkout = std::any_of(workouts.begin(), workouts.end(), [](Workout* w) {
        return w->excercises.size() > 2;
    });

    if (hasLongWorkout) {
        cout << "- You have completed at least one long workout (>2 exercises)! Good job." << endl;
    } else {
        cout << "- Keep pushing! Try adding more exercises to your workouts." << endl;
    }

    
    int totalWorkouts = std::count_if(workouts.begin(), workouts.end(), [](Workout* w) {
        return w != nullptr; 
    });
    cout << "- Total valid workout sessions saved: " << totalWorkouts << endl;

    
    cout << "\n-------------------------------------" << endl;
    string targetExcercise = getSafeInput<string>("Enter a Strength Excercise name to check PR and sets:\t");
    
    float personalRecord = 0.0f;
    int totalSetsDone = 0;

    
    std::for_each(workouts.begin(), workouts.end(), [&](Workout* w) {
        
        
        for (const auto& exPair : w->excercises) {
            if (exPair.second->name == targetExcercise) {
                
                
                StrengthExcercise* se = dynamic_cast<StrengthExcercise*>(exPair.second);
                
                if (se != nullptr) {
                    
                    totalSetsDone += std::count_if(se->sets.begin(), se->sets.end(), [](const auto& setPair) {
                        return setPair.second->weight >= 0.0f; 
                    });

                    
                    std::for_each(se->sets.begin(), se->sets.end(), [&](const auto& setPair) {
                        if (setPair.second->weight > personalRecord) {
                            personalRecord = setPair.second->weight;
                        }
                    });
                }
            }
        }
    });

    if (totalSetsDone > 0) {
        cout << "\nStats for [" << targetExcercise << "]:" << endl;
        cout << "Total sets performed: " << totalSetsDone << endl;
        cout << "Personal Record (Max Weight): " << personalRecord << " kg" << endl;
    } else {
        cout << "\nNo strength data found for excercise: " << targetExcercise << endl;
    }
    
    cout << "=====================================\n\n";
}


void User::removeWorkout()
{
    if (workouts.empty()) {
        cout << "No workouts available to remove." << endl;
        return;
    }

    displayWorkouts();
    
    int id = getSafeInput<int>("Enter the ID of the workout to remove:\t");


    if (id >= 0 && id < workouts.size()) {
        delete workouts[id]; // Usuwamy obiekt z pamięci
        workouts.erase(workouts.begin() + id); 
        cout << "Workout removed successfully!" << endl;
    } else {
        cout << "Invalid ID." << endl;
    }
}