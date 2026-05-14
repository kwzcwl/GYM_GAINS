#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

using namespace std;

void workout_menu(Workout w,User user)
{
	w.printWorkout();


	cout<<endl<<"1 - Add Excercise"<<endl;
	cout<<endl<<"2 - Add Set"<<endl;
	cout<<endl<<"3 - Finish Workout"<<endl;

	int user_input;

	cin>>user_input;

	switch(user_input)
	{
		case 1:
			w.addExcercise();
			break;

		case 2:
			w.addSet();
			break;

		case 3:
			user_menu(user);
			break;

		default:
			workout_menu(w,user);
			break;
	}

}


struct Set
{
    public:
		int reps;
        float weight;
        int RPE;
        string comment;

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
    public:
		string name;
        map<int,Set> sets;

    	Excercise(string n)
		{
			name = n;
		}
		
		void addSet()
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
		
		void printExcercise()
		{
			cout<<endl<<"--------------------------------------------"<<endl<<"\t\t"<<name<<endl<<"--------------------------------------------"<<endl;
			cout<<"SET\tREPS\tWEIGHT\tRPE\tComment"<<endl;
			
			for(auto& set : sets)
			{
				cout<<set.first<<"\t";
				
				set.second.printSet();
			}
		}
    	
};

struct Workout
{
    public:

		map<int,Excercise> excercises;
        string name;
        string date;
        string duration;

    	Workout(string n,string d)
		{
			name = n;
			date = d;
		}
		
		void addExcercise()
		{
			int id = 1;
			
			for(auto& ex : excercises)
			{
				if(ex.first==id)
				{
					id++;
				}
			}
			

			string name;
			
			cout<<endl<<"Adding "<<id<<" excersise"<<endl;
			cout<<"-----------------------"<<endl;
			cout<<"Enter the name of excercise: ";
			getline(cin,name);
			
			excercises.insert({id,Excercise(name)});
		}
		
		void printWorkout()
		{
			cout<<"\033[2J\033[1;1H";
			cout<<"============================================"<<endl;
			cout<<date<<"\t"<<name<<endl;
			cout<<"============================================"<<endl;
			for(auto& ex : excercises)
			{
				ex.second.printExcercise();
			}
		}
		
		void addSet()
		{
			cout<<endl<<"For which excercise do you want to add the set?"<<endl;
			for(auto& ex:excercises)
			{
				cout<<ex.first<<"\t-\t"<<ex.second.name<<endl;
			}

			int input;

			cin>>input;

			for(auto& ex:excercises)
			{
				if(ex.first == input)
				{
					ex.second.addSet();
					break;
				}
			}
		}
};

struct User
{
    private:
        vector<Workout> workouts;
        string password;
        
    public:
        string name;
		string surname;
        string username;

		User(string n,string sn,string passwd,string un = " ")
		{
			name = n;
			surname = sn;
			password = passwd;
			if(un == " ")
			{
				username = un;
			}
			else
			{
				username = n;
			}
		}

		void addWorkout(User user)
		{
			cout<<endl<<"Adding Workout, enter workout name:\t";
			string wname;
			getline(cin,wname);

			workouts.push_back(Workout(wname,"12.05.2025"));

			Workout& w = workouts.back();
			
			workout_menu(w,user);
		}
};

//struct Analyzer{};


/*time_t getTime() {
    auto now = system_clock::now();
    return system_clock::to_time_t(now);
}
Bench Pres
string formatDate(time_t date) {
    char buffer[80];
    // Bezpieczna konwersja czasu na lokalny
    tm* timeinfo = std::localtime(&date);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return string(buffer);
}*/

void exit_program()
{
	
}

void user_menu(User user)
{
	cout<<endl<<"0 - Back to main menu"<<endl;
	cout<<endl<<"1 - Add Workout"<<endl;
	cout<<endl<<"2 - Check old workouts"<<endl;

	int user_input;

	cin>>user_input;

	switch(user_input)
	{
		case 0:
			main_menu();
			break;

		case 1:
			user.addWorkout(user);
			break;

		case 2:
			cout<<"comming soon";
			user_menu(user);
			break;

		default:
			user_menu(user);
			break;
	}
}

void main_menu()
{
	cout<<endl<<"[M]-Menu\t[X]-Exit\t";
	string user_input;
	cin>>user_input;
	if(user_input=="M")
	{
		//user_menu();
	}
	else
	{
		if(user_input=="X")
		{
			exit_program();
		}
		else
		{
			main_menu();
		}
	}
}



int main()
{
	User myuser("Mokry","Marek","robmiloda1234");

	user_menu(myuser);

	/*myWorkout.addExcercise();
	myWorkout.printWorkout();
	myWorkout.addSet();
	myWorkout.printWorkout();
	myWorkout.addSet();
	myWorkout.printWorkout();
	myWorkout.addExcercise();
	myWorkout.printWorkout();
	myWorkout.addSet();
	myWorkout.printWorkout();*/
	
	return 0;	
}
