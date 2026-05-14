#include "Set.h"

using namespace std;



Set::Set(int r, float w, int R, string c)
{
	reps = r;
	weight = w;
	RPE = R;
	comment = c;
}    
void Set::printSet()
{
	cout<<reps<<"\t"<<weight<<" kg\t"<<RPE<<"\t"<<comment<<endl;
}
