#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

using namespace std;

struct User
{
    private:
        vector<Workout> workouts;
        string password;
        
    public:
        string name;
        string username;
    
};

struct Workout
{
    private:
        vector<Excercise> excercises;
        string date;
};

struct Excercise
{
    private:
        string name;
        map<int,Set> sets;
};

//struct WeightExcercise{};

//struct CardioExcercise{};

struct Set
{
    private:
        int reps;
        float weight;
        int RPE;
        string comment;
    
    
};

//struct Analyzer{};


time_t getTime() {
    auto now = system_clock::now();
    return system_clock::to_time_t(now);
}

string formatDate(time_t date) {
    char buffer[80];
    // Bezpieczna konwersja czasu na lokalny
    tm* timeinfo = std::localtime(&date);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return string(buffer);
}

int main()
{

	return 0;	
}