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
        static User* load(std::ifstream& ifs);

		void addWorkout(Database& db);
		void addWorkoutGUI(string wname);
        void displayWorkouts();
        bool checkPassword(string inPasswd);
	void analyzeProgress();
	void removeWorkout();

	vector<Workout*>& getWorkouts() { return workouts; }
	const vector<Workout*>& getWorkouts() const { return workouts; }
};