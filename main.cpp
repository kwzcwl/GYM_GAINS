#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

#include "Excercise.h"
#include "Menus.h"
#include "Set.h"
#include "Workout.h"
#include "User.h"

using namespace std;


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
