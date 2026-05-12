#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

using namespace std;

struct Set
{
    private:
        int reps;
        float weight;
        int RPE;
        string comment;
    public:
        Set(int r = 0, float w = 0, int R = 0, string c = "")
		{
			reps = r;
			weight = w;
			RPE = R;
			comment = c;
		}    
		void printSet()
		{
			cout<<reps<<"\t"<<weight<<" kg\t"<<RPE<<"\t"<<comment<<endl;
		}
};

struct Excercise
{
    private:
        string name;
        map<int,Set> sets;
        
    public:
    	Excercise(string n)
		{
			name = n;
		}
		
		void addSet()
		{
			int id = 1;
			
			for(auto set : sets)
			{
				if(set.first>id)
				{
					id = set.first;
				}
			}
			
			int reps;
			float weight;
			int RPE;
			string comment;
			
			cout<<endl<<"Adding "<<id<<" set of "<<name<<": "<<endl;
			cout<<"-----------------------"<<endl;
			cout<<"Enter the number of reps: "<<endl;
			cout<<"Enter weight: "<<endl;
			cout<<"Enter RPE: "<<endl;
			cout<<"Enter a comment: "<<endl;
			
			sets.insert({id,Set(reps,weight,RPE,comment)});
		}
		
		void printExcercise()
		{
			cout<<endl<<"-------------------"<<endl<<name<<endl<<"-------------------"<<endl;
			cout<<"SET\tREPS\tWEIGHT\tRPE\tComment"<<endl;
			
			for(auto set : sets)
			{
				cout<<set.first<<"\t";
				
				set.second.printSet();
			}
		}
    	
};

struct Workout
{
    private:
        map<int,Excercise> excercises;
        string name;
        string date;
        string duration;
        
    public:
    	Workout(string n,string d)
		{
			name = n;
			date = d;
		}
		
		void addExcercise()
		{
			int id = 1;
			
			for(auto ex : excercises)
			{
				if(ex.first>id)
				{
					id = ex.first;
				}
			}
			

			string name;
			
			cout<<endl<<"Adding "<<id<<" excersise"<<endl;
			cout<<"-----------------------"<<endl;
			cout<<"Enter the name of excercise: ";
			cin>>name;
			
			excercises.insert({id,Excercise(name)});
		}
		
		void printWorkout()
		{
			cout<<"======================"<<endl;
			cout<<date<<"\t"<<name<<endl;
			for(auto ex : excercises)
			{
				ex.second.printExcercise();
			}
		}
		
		void addSet()
		{
			excercises[excercises.size()].addSet();
		}
        
        
};

struct User
{
    private:
        vector<Workout> workouts;
        string password;
        
    public:
        string name;
        string username;
    
    
    
};

//struct Analyzer{};


/*time_t getTime() {
    auto now = system_clock::now();
    return system_clock::to_time_t(now);
}

string formatDate(time_t date) {
    char buffer[80];
    // Bezpieczna konwersja czasu na lokalny
    tm* timeinfo = std::localtime(&date);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return string(buffer);
}*/

int main()
{
	Workout myWorkout("mojtrening","jakas data");

	myWorkout.addExcercise();
	myWorkout.printWorkout();
	
	return 0;	
}
