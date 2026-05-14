#include "Excercise.h"
#include <iostream>

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
    
    int reps;
    float weight;
    int RPE;
    string comment;
    
    cout<<endl<<"Adding "<<id<<" set of "<<name<<": "<<endl;
    cout<<"-----------------------"<<endl;
    cout<<"Enter the number of reps: "<<endl;
    cin>>reps;
    cout<<"Enter weight: "<<endl;
    cin>>weight;
    cout<<"Enter RPE (Blank default): "<<endl;
    cin>>RPE;
    cout<<"Enter a comment (Blank default): "<<endl;
    cin.ignore();
    getline(cin,comment);
    
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
    	
