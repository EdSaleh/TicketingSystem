#include <iostream>
#include <string>
#include <vector>
#include "User.h"
#include "Event.h"
#include "Admin.h"
#include "Tools.h"
using namespace std;

//Function: Admin
//Input: String n, float c
//Called By: Main
Admin::Admin(string n, float c){
	name = n;
	credit = c;
	buyMax = 9999999;
	type = 01;
}

//Function: Admin
//Input: User u
//Called By: Transaction
Admin::Admin(User u){
	name = u.getName();
	credit = u.getCredit();
	buyMax = 9999999;
	type = 1;
}



//Function: create
//Creates a new user.
//Output: int < 0 for error
//Called By: Transaction
int Admin::create(vector<User> UsersList){
	string user, typeU;
	int typeVal;
	bool matchFound = false;
	//String typeToFile; //Only used when we actually are going to write to file
	cout << "What is the new username?\n\n";
	cout << "Name: ";
	cin >> user;

	for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
		if (user == trimName(it->getName())){
			cout << "User already exits.\n";
			return -1;
		}
	}

	cout << "What type of account does this user have?\n\n";
	cout << "Type(admin,buy,sell,full): ";
	cin >> typeU;
	if (typeU == "admin"){
		typeVal = 1;
		//typeToFile = "AA";
	}
	else if (typeU == "buy"){
		typeVal = 2;
		//typeToFile = "BS";
	}
	else if (typeU == "sell"){
		typeVal = 3;
		//typeToFile = "SS";
	}
	else if (typeU == "full"){
		typeVal = 4;
		//typeToFile = "FS";
	}

	user.insert(user.size(), 14 - user.size(), ' ');

	UsersList.push_back(User(user, typeVal, 0.0));
	WriteUsers(UsersList);
	//Write to TransactionFile

	string ty = to_string(typeVal);
	string cr = to_string(0.0);
	ty.insert(0, 1, '0');
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 11 - cr.size(), '0');
	addToTrans("01 " + user + " " + ty + " " + cr);

	return 0;
	
}

//Function: del
//Deletes a user from the list
//Output: x<0 if an error occurs
//Called By: Transaction
//Calls: Tools
int Admin::del(vector<User> UsersList, vector<Event> EventsList){
	string user; //The variable space where we will store the name of the targeted user
	bool matchFound = false; //We don't know if the target exists yet.
	vector<User>::iterator delTarget;
	vector<Event>::iterator delEvent;
	int delType = 0;
	float delCred = 0.0;
	string delName = "";
	cout << "Which user are you going to delete?\n\n";
	cout << "User: ";
	cin >> user;
	//Can't delete self
	if (user == name){
		cout << "You cannot delete yourself.\n\n";
		return -1;
	}

	//Finding the user.
	for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
		if (user == trimName(it->getName())){
			matchFound = true;
			delType = it->getType();
			delCred = it->getCredit();
			delName = it->getName();
			delTarget = it;
		}
	}
	if (matchFound == false){ //Bailing if the user does not exist.
		cout << "User does not exist\n";
		return -1;
	}
	UsersList.erase(delTarget);
	
	bool noEvents = false;
	while (noEvents == false){
		noEvents = true;
		for (vector<Event>::iterator it = EventsList.begin(); it != EventsList.end(); ++it){
			if (name == trimName(it->getSeller())){
				delEvent = it;
				noEvents = false;
				break;
			}
		}
		if (noEvents == false){
			EventsList.erase(delEvent);
		}
	}
	WriteEvent(EventsList);
	WriteUsers(UsersList);
	//XX_UUUUUUUUUUUUU_TT_CCCCCCCCC
	//Create, Delete, addCredit, End of Session
	//01,02,06,00
	//TransactionFile << '0' << type << ' ' << name << " " << usertype << ' ' << credit;  

	string ty = to_string(delType);
	string cr = to_string(delCred);
	ty.insert(0, 1, '0');
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 11 - cr.size(), '0');
	addToTrans("02 " + delName + " " + ty + " " + cr);


	//Doesn't delete from local storage, delete their tickets from the events, or write to file
	return 0;
}

//Function: Refund
//Takes credit from a seller and gives it to a user
//Output: int < 0 if there is an error
//Called By: Transactions
//Calls: User
int Admin::refund(vector<User> UsersList){
	User BuyUser;  //User recieving credit
	User SellUser; //User losing credit
	string input; 
	float amount; 

	bool matchFound = false;

	cout << "Which user will be recieving the refund?\n";
	cout << "User:";
	cin >> input;
	//Find the user that is getting the refund
	for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
		if (input == trimName(it->getName())){
			BuyUser = (*it);
			matchFound = true;
		}
	}
	if (matchFound == false){ //Bails if user doesn't exist
		cout << "That user doesn't exist\n\n";
		return -1;
	}
	if (BuyUser.getType() == 3){ //Bails if that user couldn't buy in the first place
		cout << "That user doesn't have buy privledges. \n\n";
		return -1;
}

	cout << "Which user will be refunding the money?\n";
	cout << "User:";
	cin >> input;
	//Find the user that is giving the refund
	for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
		if (input == trimName(it->getName())){
			SellUser = (*it);
			matchFound = true;
		}
	}
	if (matchFound == false){ //Bails if user doesn't exist
		cout << "That user doesn't exist\n\n";
		return -1;
	}
	if (SellUser.getType() == 2){ //Bails if user doesn't have privilege to sell in the first place
		cout << "That user doesn't have sell privledges. \n\n";
		return -1;
	}
	cout << "How much money is being refunded? \n\n"; 
	cout << "Amount: ";
	cin >> amount;

	if (amount > SellUser.getCredit()){ //Prevents someone from refunding more money then they have
		cout << "The seller does not have enough money.";
		return - 1;
	}

	if (BuyUser.addCredit(amount) < 0){
		return -1;
	}//Adds money to user
	if (SellUser.addCredit(-1 * amount) < 0){
		BuyUser.addCredit(-1 * amount);
		return -1;
	}//Subtracts money from other user
	
	//Write to file

	
	string cr = to_string(amount);
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 11 - cr.size(), '0');
	addToTrans("05 " + BuyUser.getName() + " " + SellUser.getName() + " " + cr);

	//XX_UUUUUUUUUUUUU_UUUUUUUUUUUUU_CCCCCCCCC
	//Refund
	//05
	//TransactionFile << '0' << type << ' ' << name << " " << name << ' ' << credit;  


	return 0;
}

//Function: addCredit
//The admin version of add credit, allows an admin to add credit to any user account
//Output: returns int < 0 if there is an error
//Called By: Transaction
//Calls: User, Tools
int Admin::addCredit(vector<User> UsersList){
	string input;
	float amount;
	User userTarget;

	bool matchFound = false;

	cout << "Please enter the name of the User to add credit to. \n\n";
	cout << "User: ";
		cin >> input;
	//Checking to find the user
	for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
		if (input == it->getName()){
			userTarget = (*it);
			matchFound = true;
		}
	}
	if (matchFound == false){ //Bails if user doesn't exist
		cout << "That user doesn't exist\n\n";
		return -1;
	}

	//Askes for ammount
	cout << "Please enter the amount. \n\n\n\n";
	cout << "$:";
	cin >> amount;
	
	//Runs the (user)addCredit
	if (userTarget.addCredit(amount) < 0){
		return -1;
	}
		
	string ty = to_string(userTarget.getType());
	string cr = to_string(userTarget.getType());
	ty.insert(0, 1, '0');
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 11 - cr.size(), '0');
	addToTrans("06 " + userTarget.getName() + " " + ty + " " + cr);
	return 0;
}