#pragma once
#include <string>
#include <map>
#include "Set.h"
#include <fstream>

using namespace std;

class Excercise
{
public:
    string name;

    Excercise(string n) : name(n) {}
    virtual ~Excercise() = default; 

    virtual void save(ofstream& ofs) = 0;
    virtual void addSet() = 0; 
    virtual void printExcercise() = 0;
    virtual string getType() const = 0; 

    static Excercise* load(ifstream& ifs);
};

class StrengthExcercise : public Excercise
{
public:
    map<int, Set*> sets;

    StrengthExcercise(string n);
    ~StrengthExcercise() override;

    void save(ofstream& ofs) override;
    void addSet() override;
    void printExcercise() override;
    string getType() const override { return "Strength"; }
};

class Cardio : public Excercise
{
public:
    float distance; 
    int duration; 

    Cardio(string n, float dist = 0.0f, int dur = 0);
    ~Cardio() override = default;

    void save(ofstream& ofs) override;
    void addSet() override;
    void printExcercise() override;
    string getType() const override { return "Cardio"; }
};
