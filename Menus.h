#pragma once
#include <string>


struct User;
struct Workout;

using namespace std;

int getSafeIntInput(string prompt);

void workout_menu(Workout& w,User& user);
void exit_program();
void user_menu(User& user);
void main_menu();