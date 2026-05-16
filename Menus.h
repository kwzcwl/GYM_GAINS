#pragma once
#include <string>
#include "Database.h"


struct User;
struct Workout;

using namespace std;

int getSafeIntInput(string prompt);

void workout_menu(Workout& w,User& user,Database& db);
void exit_program();
void user_menu(User& user,Database& db);
void main_menu(Database& db);