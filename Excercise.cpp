#include "Excercise.h"
#include <iostream>
#include "Inputs.h"

Excercise::Excercise(string n)
{
    name = n;
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

    /*cout<<"Enter the number of reps: "<<endl;
    cin>>reps;
    cout<<"Enter weight: "<<endl;
    cin>>weight;
    cout<<"Enter RPE (Blank default): "<<endl;
    cin>>RPE;
    cout<<"Enter a comment (Blank default): "<<endl;
    cin.ignore();
    getline(cin,comment);*/

    int reps = getSafeInput<int>("Enter the number of reps:\t");
    float weight = getSafeInput<float>("Enter weight (kg):\t");
    int RPE = getSafeInput<int>("Enter RPE (Blank default):\t");
    string comment = getSafeInput<string>("Enter a comment (Blank default):\t");

    

    
    sets.insert({id,Set(reps,weight,RPE,comment)});
}

void Excercise::printExcercise()
{
    cout<<endl<<"--------------------------------------------"<<endl<<"\t\t"<<name<<endl<<"--------------------------------------------"<<endl;
    cout<<"SET\tREPS\tWEIGHT\tRPE\tComment"<<endl;
    
    for(auto& set : sets)
    {
        cout<<set.first<<"\t";
        
        set.second.printSet();
    }
}
    	
