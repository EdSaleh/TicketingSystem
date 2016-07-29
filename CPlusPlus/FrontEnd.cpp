#include <iostream>
#include <iomanip>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>
#include "Event.h"
#include "User.h"
#include "Admin.h"
#include "Tools.h"
using namespace std;


//Function: Transaction
//Main loop where user is asked to do transactions
//Called By: Main
//Calls To: User, Admin
void transaction(vector<User> UsersList, vector<Event> EventsList){
	bool loggedIn = false;
	User CurrentUser;
	bool isAdmin = false;
	Admin CurrentAdmin; //Incase the user is an admin and they have admin privledges
	string UserName;

	bool loopCondition = true;	//a condition initilized to true for the while loop to indefinitely repeat

	string input;	//Input from users will be stored in this variable
	float amount;	//Float amounts in inputs will be stored here

	//File io for the transaction file, writting to transaction file not yet implemented.

	//The loop where the user is asked about performing a transaction
	while (loopCondition){
		cout << "Which kind of transaction would you like to perform?\n";
		cout << "Transaction: ";
		cin >> input;
		
		//QUIT
		if (input == "quit"){
			if (loggedIn == true){
				cout << "Please logout first. \n\n";
			}
			else{
				loopCondition = false;
			}
		}
		else if (loggedIn == false && (input != ("login"))){
			cout << "You must login first. \n\n";
		}
		else{
			//Login
			if (input == "login"){
				cout << "Enter your user name: \n Name:";
				cin >> input;
				for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){ //Searches through Users list
					UserName = trimName(it->getName()); //Sends the string of the name to get endspaces trimmed off to better compare.
					if (input == UserName){
						CurrentUser = (*it); //Setting current user
						loggedIn = true;	//Confirming that user can now use other transactions
						//Updating Events list
						if (CurrentUser.getType() == 01){	//Checks if user is an Admin
							isAdmin = true;
						}
						cout << "You have successfully logged in.\n";
						EventsList = UpdateEvents();
					}
				}
			}
			//Logout
			else if (input == "logout"){
				loggedIn = false;	//Makes the user have to log back in to access other transactions
				isAdmin = false;	//Prevents accidental admin
				
				//Writes to file. Really broken.
				//Doesn't put enough spacing for some credit, Doesn't store proper types for users privliges. Doesn't append.
				ofstream TransactionFile("TransactionFile.txt", ios::app);
				TransactionFile << getTrans();
				string cred = to_string(CurrentUser.getCredit());
				cred = cred.substr(0, cred.size() - 4);
				cred.insert(0, 11 - cred.size(), '0');
				TransactionFile << "00 " << CurrentUser.getName() << " 0" << CurrentUser.getType() << ' ' << cred << endl;
				TransactionFile.close(); 
				cout << "You have successfully logged out. \n";
				UsersList = UpdateUsers();
			}
			//Create
			else if (input == "create" && isAdmin){
				CurrentAdmin.create(UsersList); //Admin only, Calls Admin.create
				UsersList = UpdateUsers();
			}
			//Delete
			else if (input == "delete" && isAdmin){
				CurrentAdmin.del(UsersList, EventsList);	//Admin only, Calls Admin.del
				UsersList = UpdateUsers();
			}
			//Sell
			else if (input == "sell"){
				CurrentUser.sell(UsersList, EventsList);	//Calls User.sell
			}
			//Buy
			else if (input == "buy"){
				CurrentUser.buy(UsersList, EventsList);		//Calls User.Buy
			}
			//Refund
			else if (input == "refund" && isAdmin){			//Admin only, calls Admin.refund
				CurrentAdmin.refund(UsersList);
			}
			//Add Credit
			else if (input == "addCredit" || input == "add" || input == "addcredit"){	//Works for two cases.
				if (isAdmin){									//Checks if user is admin to run the admin version or not
					CurrentAdmin.addCredit(UsersList);			//Admin only, calls Admin.addCredit
				}
				else{
					cout << "Enter how much you wish to add.\n $:";
					cin >> amount;
					CurrentUser.addCredit(amount);	//Calls the basic user one, User.addCredit
					//Then alerts user to how much money they have.
					//Doesn't currently check if they hit maximum, Doesn't check if they try to add more then allowed
					//Doesn't currently check if they enter negative
					cout << "You now have $" << fixed << setprecision(2) << CurrentUser.getCredit() << endl; 
				}
			}
			else{
				cout << "Please enter a valid transaction.\n";
			}
		}
	}
}

//Function: Main
//Main function to initilize the user base and event base, and then call the transactions
//Called by: User
//Calls: Transaction

int main()
{
	//Iniitilizing important vectors used for our userbase and tickets base
	vector<User> UsersList = UpdateUsers();	
	vector<Event> EventsList = UpdateEvents();
	
	transaction(UsersList, EventsList);

	return 0;
}


// Data for when implemnting writing to the transaction file: 

//XX_UUUUUUUUUUUUU_TT_CCCCCCCCC
//Create, Delete, addCredit, End of Session
//01,02,06,00
//TransactionFile << '0' << type << ' ' << name << " " << usertype << ' ' << credit;  



//XX_UUUUUUUUUUUUU_UUUUUUUUUUUUU_CCCCCCCCC
//Refund
//05
//TransactionFile << '0' << type << ' ' << name << " " << name << ' ' << credit;  


//XX_EEEEEEEEEEEEEEEEEEEE_SSSSSSSSSSSSS_TTT_PPPPPP
//Buy, Sell
//03,04
//TransactionFile << '0' << type << ' ' << event << ' ' << name << " " << tickets << ' ' << price;  
