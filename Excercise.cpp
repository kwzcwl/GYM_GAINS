#include "Excercise.h"
#include <iostream>
#include "Inputs.h"
#include <fstream>

using namespace std;

Excercise::Excercise(string n)
{
    name = n;
}

Excercise::~Excercise() {
    for (auto& s : sets)
    {
        delete s.second;
    }
    sets.clear();
}

Excercise::Excercise(const Excercise& other)
{
    this->name = other.name;
    
    for (const auto& pair : other.sets)
    {
        if (pair.second != nullptr)
        {
            this->sets[pair.first] = new Set(pair.second->reps, pair.second->weight);
        }
    }
}

Excercise& Excercise::operator=(const Excercise& other) {
    if (this == &other)
    {
        return *this;
    }

    for (auto& pair : this->sets)
    {
        delete pair.second;
    }
    this->sets.clear();

    this->name = other.name;

    for (const auto& pair : other.sets)
    {
        if (pair.second != nullptr)
        {
            this->sets[pair.first] = new Set(pair.second->reps, pair.second->weight);
        }
    }

    return *this;
}


void Excercise::addSet()
{
    int id = 1;
    
    for(auto set : sets)
    {
        if(set.first==id)
        {
            id++;
        }
    }
    
    
    
    cout<<endl<<"Adding "<<id<<" set of "<<name<<": "<<endl;
    cout<<"-----------------------"<<endl;

    int reps = getSafeInput<int>("Enter the number of reps:\t");
    float weight = getSafeInput<float>("Enter weight (kg):\t");
    int RPE = getSafeInput<int>("Enter RPE (Blank default):\t");
    string comment = getSafeInput<string>("Enter a comment (Blank default):\t");

    

    sets[id] = new Set(reps,weight,RPE,comment);
}

void Excercise::save(ofstream& ofs)
{
    ofs<<name<<"\n";
    ofs<<sets.size()<<"\n";

    for (auto& s : sets)
    {
        ofs<<s.first<<" "<<s.second->reps<< " " <<s.second->weight<<" "<<s.second->RPE<<" "<<s.second->comment<<"\n";
    }
}


Excercise* Excercise::load(std::ifstream& ifs) {
    std::string exName;

    if (!std::getline(ifs, exName))
    {
        return nullptr;
    }

    Excercise* newExercise = new Excercise(exName);
    
    size_t mapSize = 0;
    ifs >> mapSize;
    ifs.ignore();

    for (size_t i = 0; i < mapSize; ++i)
    {
        int key = 0;
        int reps = 0;
        float weight = 0.0f;
        

        ifs >> key >> reps >> weight;
        ifs.ignore();
        
        newExercise->sets[key] = new Set(reps, weight);
    }

    return newExercise;
}


void Excercise::printExcercise()
{
    cout<<endl<<"--------------------------------------------"<<endl<<"\t\t"<<name<<endl<<"--------------------------------------------"<<endl;
    cout<<"SET\tREPS\tWEIGHT\tRPE\tComment"<<endl;
    
    for(auto& set : sets)
    {
        if(set.second != nullptr)
        {
            cout<<set.first<<"\t";
        
            set.second->printSet();
        }
    }
}
    	
