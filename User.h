#pragma once
#include <vector>
#include <string>
#include "Workout.h"

string getDate();

struct User
{
    private:
        vector<Workout> workouts;
        string password;
        
    public:
        string name;
		string surname;
        string username;

		User(string n,string sn,string passwd,string un = " ");
		void addWorkout(User& user);
        void displayWorkouts();
};