#include "Menus.h"
#include "User.h"
#include "Workout.h"
#include "Inputs.h"
#include "Database.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;


User* signin(Database& db)
{
    string inUsername, inPasswd;
    cout << "Username: ";
    getline(cin, inUsername);
    
    inPasswd = getHiddenInput("Password: ");

    for (User* u : db.getUsers())
    {
        if (u != nullptr && u->username == inUsername && u->checkPassword(inPasswd))
        {
            return u;
        }
    }

    return nullptr;
}


void workout_menu(Workout& w, User& user, Database& db)
{
    bool in_workout = true;

    while (in_workout)
    {
        w.printWorkout();
        
        string prompt = "[0] Add Excercise\t[1] Add Set\t[2] Edit Set\t[3] Remove Set\t[4] Remove Excercise\t[5] Finish Workout\nChoice:\t";

        switch(getSafeInput<int>(prompt))
        {
            case 0:
                w.addExcercise(w, user);
                break;
            case 1:
                w.addSet(w, user);
                break;
            case 2:
                w.editSetFromWorkout(); 
                break;
            case 3:
                w.removeSetFromWorkout(); 
                break;
            case 4:
                w.removeExcercise(); 
                break;
            case 5:
                cout << "\nSaving and finishing workout...\n" << endl;
                in_workout = false;
                break;
            default:
                break;
        }
    }
}

void exit_program()
{
    cout << "Exiting program. Goodbye!" << endl;
}

void user_menu(User& user, Database& db)
{
    bool in_profile = true;

    while (in_profile)
    {
        switch(getSafeInput<int>("[0] - Add Workout\t[1] - Check history\t[2] - Check Progress\t[3] - Remove Workout\t[4] - Sign out\t:\t"))
        {
            case 0:
                user.addWorkout(db); 
                break;
            case 1:
                user.displayWorkouts();
                break;
            case 2:
                user.analyzeProgress();
                break;
            case 3:
                user.removeWorkout(); 
                break;
            case 4:
                cout << "Signing out...\n\n";
                in_profile = false;
                break; 
            default:
                break;
        }
    }
}
User* signup()
{
    cout << endl << "Name:\t";
    string name;
    getline(cin, name);

    cout << endl << "Surname:\t";
    string surname;
    getline(cin, surname);

    cout << endl << "Username:\t";
    string username;
    getline(cin, username);

    string password1 = getHiddenInput("Password:\t");
    string password2 = getHiddenInput("Confirm password:\t");

    while (password1 != password2)
    {
        cout << endl << "Passwords do not match!" << endl;
        password1 = getHiddenInput("Password:\t");
        password2 = getHiddenInput("Confirm password:\t");
    }

    cout << endl << "User successfully added!" << endl << endl;

    return new User(name, surname, password1, username);
}

void main_menu(Database& db)
{
    bool running = true;

    while (running) 
    {
        switch(getSafeInput<int>("[0] - Sign in\t[1] - Sign up\t[2] - Exit app\t:\t"))
        {
            case 0:
            {
                User* user = signin(db);
                if (user != nullptr)
                {
                    cout << "Login successful! Welcome " << user->name << endl;
                    user_menu(*user, db);
                }
                else
                {
                    cout << "Invalid username or password." << endl;
                }
                break;
            }
            case 1:
            {
                User* new_user = signup(); 
                db.addUser(new_user);      
                user_menu(*new_user, db);  
                break;
            }
            case 2:
                exit_program();
                running = false;
                break;
            default:
			break;
        }
    }
}