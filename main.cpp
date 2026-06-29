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
#include "Database.h"

using namespace std;


int main()
{
	Database My_Database("Data.txt");

	My_Database.load();

	main_menu(My_Database);
	
	My_Database.save();

	return 0;	
}
