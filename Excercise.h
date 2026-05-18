#pragma once
#include <string>
#include <map>
#include "Set.h"
#include <fstream>

using namespace std;

// Abstrakcyjna klasa bazowa
class Excercise
{
public:
    string name;

    Excercise(string n) : name(n) {}
    virtual ~Excercise() = default; // Wirtualny destruktor to podstawa przy polimorfizmie

    // Czysto wirtualne metody (każda podklasa musi je zaimplementować)
    virtual void save(ofstream& ofs) = 0;
    virtual void addSet() = 0; // W przypadku cardio to może być po prostu edycja parametrów
    virtual void printExcercise() = 0;
    virtual string getType() const = 0; // Przyda się do zapisu/odczytu z pliku

    // Metoda fabrykująca
    static Excercise* load(ifstream& ifs);
};

// Podklasa dla ćwiczeń siłowych (korzysta z Twojej klasy Set)
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

// Podklasa dla Cardio
class Cardio : public Excercise
{
public:
    float distance; // w km
    int duration; // w minutach

    Cardio(string n, float dist = 0.0f, int dur = 0);
    ~Cardio() override = default;

    void save(ofstream& ofs) override;
    void addSet() override;
    void printExcercise() override;
    string getType() const override { return "Cardio"; }
};