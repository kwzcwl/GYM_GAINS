#pragma once
#include <vector>
#include <string>
#include "Workout.h"

string getDate();

class Database;

struct User
{
    private:
        vector<Workout*> workouts;
        string password;
        
    public:
        string name;
		string surname;
        string username;

		User(string n,string sn,string passwd,string un = " ");
        ~User();

        void save(std::ofstream& ofs);
        //User* load(std::ifstream& ifs);

		void addWorkout(Database& db);
        void displayWorkouts();
        bool checkPassword(string inPasswd);
};