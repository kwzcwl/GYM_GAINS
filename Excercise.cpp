#include "Excercise.h"
#include <iostream>
#include "Inputs.h"

using namespace std;

StrengthExcercise::StrengthExcercise(string n) : Excercise(n) {}

StrengthExcercise::~StrengthExcercise() {
    for (auto& s : sets) {
        delete s.second;
    }
    sets.clear();
}

void StrengthExcercise::addSet() {
    int id = sets.empty() ? 1 : sets.rbegin()->first + 1;
    cout << "\nAdding " << id << " set of " << name << ": \n";
    
    int reps = getSafeInput<int>("Enter the number of reps:\t");
    float weight = getSafeInput<float>("Enter weight (kg):\t");
    int RPE = getSafeInput<int>("Enter RPE (Blank default):\t");
    string comment = getSafeInput<string>("Enter a comment (Blank default):\t");

    sets[id] = new Set(reps, weight, RPE, comment);
}

void StrengthExcercise::save(ofstream& ofs) {
    ofs << getType() << "\n" << name << "\n" << sets.size() << "\n";
    for (auto& s : sets) {
        ofs << s.first << " " << s.second->reps << " " << s.second->weight << " " 
            << s.second->RPE << " " << s.second->comment << "\n";
    }
}

void StrengthExcercise::printExcercise() {
    cout << "\n--- STRENGTH: " << name << " ---\n";
    cout << "SET\tREPS\tWEIGHT\tRPE\tComment\n";
    for(auto& set : sets) {
        if(set.second != nullptr) {
            cout << set.first << "\t";
            set.second->printSet();
        }
    }
}

Cardio::Cardio(string n, float dist, int dur) : Excercise(n), distance(dist), duration(dur) {}

void Cardio::addSet() {
    cout << "\nUpdating Cardio: " << name << "\n";
    distance = getSafeInput<float>("Enter distance (km):\t");
    duration = getSafeInput<int>("Enter duration (minutes):\t");
}

void Cardio::save(ofstream& ofs) {
    ofs << getType() << "\n" << name << "\n" << distance << " " << duration << "\n";
}

void Cardio::printExcercise() {
    cout << "\n--- CARDIO: " << name << " ---\n";
    cout << "Distance: " << distance << " km \t Duration: " << duration << " min\n";
}

Excercise* Excercise::load(std::ifstream& ifs) {
    std::string type;
    
    
    if (!std::getline(ifs, type)) {
        return nullptr;
    }

    
    std::string exName;
    if (!std::getline(ifs, exName)) {
        return nullptr;
    }

    
    if (type == "Strength") {
        StrengthExcercise* newExercise = new StrengthExcercise(exName);
        
        size_t mapSize = 0;
        ifs >> mapSize;
        ifs.ignore(); 

        for (size_t i = 0; i < mapSize; ++i) {
            int key = 0, reps = 0, rpe = 0;
            float weight = 0.0f;
            std::string comment = "";
            
            
            ifs >> key >> reps >> weight >> rpe;
            
            
            std::getline(ifs >> std::ws, comment);
            
            newExercise->sets[key] = new Set(reps, weight, rpe, comment);
        }
        return newExercise;
    } 
    else if (type == "Cardio") {
        float distance = 0.0f;
        int duration = 0;
        
        
        ifs >> distance >> duration;
        ifs.ignore(); 
        
        return new Cardio(exName, distance, duration);
    }

    
    return nullptr;
}



void StrengthExcercise::editSet()
{
    if (sets.empty()) {
        cout << "No sets to edit." << endl;
        return;
    }
    
    printExcercise();
    int id = getSafeInput<int>("Enter Set ID to edit:\t");
    
    if (sets.find(id) != sets.end()) {
        cout << "--- Editing Set " << id << " ---" << endl;
        sets[id]->reps = getSafeInput<int>("New reps:\t");
        sets[id]->weight = getSafeInput<float>("New weight (kg):\t");
        sets[id]->RPE = getSafeInput<int>("New RPE:\t");
        sets[id]->comment = getSafeInput<string>("New comment:\t");
        cout << "Set updated successfully!" << endl;
    } else {
        cout << "Set ID not found." << endl;
    }
}

void StrengthExcercise::removeSet()
{
    if (sets.empty()) {
        cout << "No sets to remove." << endl;
        return;
    }
    
    printExcercise();
    int id = getSafeInput<int>("Enter Set ID to remove:\t");
    
    if (sets.find(id) != sets.end()) {
        delete sets[id]; 
        sets.erase(id);  
        cout << "Set removed successfully!" << endl;
    } else {
        cout << "Set ID not found." << endl;
    }
}


void Cardio::editSet()
{
    
    cout << "\nEditing Cardio: " << name << "\n";
    addSet(); 
}

void Cardio::removeSet()
{
    distance = 0.0f;
    duration = 0;
    cout << "Cardio parameters reset to 0." << endl;
}
