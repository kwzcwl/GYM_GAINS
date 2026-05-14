#include <iostream>
using namespace std;

int main()
{
	int option;
	while(true)
	{
 		cout << "-----------------------------------------"<<"GYM_GAINS"<<"-------------------------------------"<<endl;
		cout << "1-SIGN IN"<<endl;
		cout << "2-CREATE ACCOUNT"<<endl;
		cin >> option;
		if(option == 1)
		{
			string username="";
			string password="";
			cout << "Enter UserName"<<endl;
			cin>>username;
			cout << "Enter Password"<<endl;
			cin>>password;
 		}
		else
		{
			string username="";
			string password="";
			cout << "Enter UserName"<<endl;
			cin>>username;
			cout << "Enter Password"<<endl;
			cin>>password;
			cout<<"confirm password"<<endl;
			string confirmation="";
			cin>>confirmation;
			if(password == confirmation)
			{					
			    cout<<"password is correct"<<endl<<"You created an account"<<endl;
			}
			else
			{					
			    cout<<"password is not the same"<<endl<<"Enter your password again"<<endl;
				cin>>confirmation;
			}
			
			
		}
 	}
}