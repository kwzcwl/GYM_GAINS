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
    w.printWorkout();

    switch(getSafeInput<int>("[0] - Add Excercise\t[1] - Add Set\t[2] - Finish Workout\t:\t"))
    {
        case 0:
            w.addExcercise(w, user);
            workout_menu(w, user, db);
            break;

        case 1:
            w.addSet(w, user);
            workout_menu(w, user, db);
            break;

        case 2:
            user_menu(user, db);
            break;

        default:
            workout_menu(w, user, db);
            break;
    }
}

void exit_program()
{
    cout << "Exiting program. Goodbye!" << endl;
}

/*void user_menu(User& user, Database& db)
{
    switch(getSafeInput<int>("[0] - Add Workout\t[1] - Check workout history\t[2] - Check Progress\t[3] - Sign out\t:\t"))
    {
        case 0:
            user.addWorkout(db);
            user_menu(user, db);
            break;

        case 1:
            user.displayWorkouts();
            user_menu(user, db);
            break;

        case 2:
            cout << "not yet implemented\n\n";
            user_menu(user, db);
            break;

        case 3:
            main_menu(db);
            break;

        default:
            user_menu(user, db);
            break;
    }
}*/


void user_menu(User& user, Database& db)
{
    bool in_profile = true;

    while (in_profile)
    {
        switch(getSafeInput<int>("[0] - Add Workout\t[1] - Check workout history\t[2] - Check Progress\t[3] - Sign out\t:\t"))
        {
            case 0:
                user.addWorkout(db); 
                break;

            case 1:
                user.displayWorkouts();
                break;

            case 2:
                cout << "not yet implemented\n\n";
                break;

            case 3:
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

/*void main_menu(Database& db)
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
                main_menu(db);
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
            break;

        default:
            main_menu(db);
            break;
    }
}*/

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