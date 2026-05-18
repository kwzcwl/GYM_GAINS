#include "Workout.h"
#include "User.h"
#include "Menus.h"
#include "Inputs.h"
#include <fstream>

using namespace std;


Workout::Workout(string n,string d)
{
	name = n;
	date = d;
}

Workout::~Workout()
{
	for (auto& e : excercises)
    {
        delete e.second;
    }
    excercises.clear();
}

void Workout::save(ofstream& ofs)
{
	ofs << name << "\n" << date << "\n";
    ofs << excercises.size() << "\n";
    for (const auto& e : excercises)
	{
        ofs << e.first << "\n";
        e.second->save(ofs);
    }
}


Workout* Workout::load(std::ifstream& ifs)
{
    std::string wName, wDate;
    if (!std::getline(ifs, wName) || !std::getline(ifs, wDate))
	{
        return nullptr;
    }

    Workout* newWorkout = new Workout(wName, wDate);
    
    size_t exerciseCount = 0;
    ifs >> exerciseCount;
    ifs.ignore();

    for (size_t i = 0; i < exerciseCount; ++i)
	{
        int mapKey = 0;
        ifs >> mapKey;
        ifs.ignore();
        
        Excercise* loadedExercise = Excercise::load(ifs);
        if (loadedExercise != nullptr)
		{
            newWorkout->excercises[mapKey] = loadedExercise;
        }
    }

    return newWorkout;
}

void Workout::addExcercise(Workout& w, User& u)
{
	int id = 1;
	
	
	for(auto& ex : excercises)
	{
		if(ex.first == id)
		{
			id++;
		}
	}
	
	cout << endl << "Adding " << id << " excersise" << endl;
	cout << "-----------------------" << endl;

	string name = getSafeInput<string>("Enter the name of excercise: ");
	

    int type = getSafeInput<int>("Select excercise type [0] Strength, [1] Cardio:\t");

    if (type == 0)
    {
        
        excercises[id] = new StrengthExcercise(name);
    }
    else if (type == 1)
    {
        
        excercises[id] = new Cardio(name);
    }
    else
    {
        
        cout << "Invalid choice. Defaulting to Strength Excercise." << endl;
        excercises[id] = new StrengthExcercise(name);
    }
}
void Workout::printWorkout()
{
	cout<<"\033[2J\033[1;1H";
	cout<<"============================================"<<endl;
	cout<<date<<"\t"<<name<<endl;
	cout<<"============================================"<<endl;
	for(auto& ex : excercises)
	{
		ex.second->printExcercise();
	}
}

void Workout::addSet(Workout& w, User& u)
{
	for(auto& ex:excercises)
	{
		cout<<ex.first<<"\t-\t"<<ex.second->name<<endl;
	}

	int input = getSafeInput<int>("For which excercise do you want to add the set?\t");

	for(auto& ex:excercises)
	{
		if(ex.first == input)
		{
			ex.second->addSet();
			break;
		}
	}
}
