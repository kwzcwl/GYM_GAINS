#include <iostream>

using namespace std;



int menulogin() // dodaæ excption i aerror handling
{
	cout<<"1 - Login"<<endl<<"2 - Create account"<<endl;
	int input = '';
	cin>>input;
	
	switch (input)
	{
		case 1:
			
			break;
		case 2:

			break;
		default:
			cout<<"Invalid input: "<<input<<endl;
			return menulogin();
	}
	
}

int main()
{
	cout<<"=============================="<<endl<<"/tGYM GAINS APP/t"<<endl<<"=============================="<<endl;
	
	int i = 0;

	while(i != 0)
	{
		i = menulogin();
	}
	
	return 0;	
}
