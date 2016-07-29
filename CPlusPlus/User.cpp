#include <iostream>
#include <string>
#include <vector>
#include "tools.h"
#include "Event.h"
#include "User.h"
using namespace std;

//Function: User 
//Input: String n, int t, float c
//Called From: Transaction, Admin
User::User(string n, int t, float c){
	name = n;
	type = t;
	credit = c;
	buyMax = 4;
}

//The get functions, to return our private values =============
string User::getName(){
	return name;
}
int User::getType(){
	return type;
}
float User::getCredit(){
	return credit;
}
//==============================================================

//Function: addCredit
//Simply takes in a number and adds that to the user's credit
//Input: float
//Output: int <0 if error
//Called by Transaction, Admin
int User::addCredit(float a){
	if (a > 1000.00){ //Prevents adding more then the maximum add
		cout << "Cannot add more than $1000.00\n\n";
		return -1;
	}
	else if ((credit + a) > 999999.00){
		cout << "This would exced credit limit of $999,999.00 on this account\n\n";
		return -1;
	}
	else if((credit + a) < 0){
		cout << "This would put this user at a negative balance.";
		return -1;
	}
	credit += a;

	//XX_UUUUUUUUUUUUU_TT_CCCCCCCCC
	//Create, Delete, addCredit, End of Session
	//01,02,06,00
	string ty = to_string(type);
	string cr = to_string(credit);
	ty.insert(0, 1, '0');
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 11 - cr.size(), '0');
	addToTrans("06 "+ name + " " + ty + " " + cr);

	return 0;
	//Write to Transaction File

}

//Function: Sell
//
//Output: int <0 for errors
//Called by: Transaction
int User::sell(vector<User> UsersList, vector<Event> EventsList){
	string title;
	int amount;
	float price;

	if (type == 2){
		cout << "You do not have sell privledges." << endl;
		return -1;
	}
	cout << "Enter the name of the event.\n";
	cin >> title;
	for (vector<Event>::iterator it = EventsList.begin(); it != EventsList.end(); ++it){
		if (title == trimName(it->getTitle()) && name == trimName(it->getSeller())){
			cout << "This event already has tickets from this user.\n";
			return -1;
		}
	}

	cout << "Enter the price per ticket\n";
	cin >> price;
	if (price < 0 || price > 999.99){
		cout << "Error: Tickets must be between 0 and 999.99\n";
		return -1;
	}
	cout << "Enter the number of tickets\n";
	cin >> amount;
	if (amount < 1 || amount > 999){
		cout << "The amount of tickets must be between 1 and 999\n";
		return -1;
	}
	title.insert(title.size(), 19 - title.size(), ' ');
	EventsList.push_back(Event(title, name, amount, price));
	WriteEvent(EventsList);
	return 0;
	//Write to file

	//XX_EEEEEEEEEEEEEEEEEEEE_SSSSSSSSSSSSS_TTT_PPPPPP
	//Buy, Sell
	//03,04
	//TransactionFile << '0' << type << ' ' << event << ' ' << name << " " << tickets << ' ' << price;  

	string ty = to_string(amount);
	string cr = to_string(price);
	ty.insert(0, 3-ty.size(), '0');
	cr = cr.substr(0, cr.size() - 4);
	cr.insert(0, 6 - cr.size(), '0');
	addToTrans("03 " + title + " " + name + " " + ty + " " + cr);

}

//Function: Buy
//Allows users to buy tickets
//Output: int <0 for errors
//Called by: Transaction
int User::buy(vector<User> UsersList, vector<Event> EventsList){
	string input, eventTitle, sellerName;
	vector<Event>::iterator currentEvent;

	//Initilized that we have not yet found the event, since we don't even know what we're looking for yet.
	bool matchFound = false; 
	
	int NumTickets;
	float pricePerTicket, totalPrice;
	
	//Make sure that a user isn't a sell only privledge
	if (type != 3){
		cout << "Which event would you like to buy for?" << endl;
		cout << "Event Name: ";
		cin >> input;
		//Making sure the event exists =====================================================
		for (vector<Event>::iterator it = EventsList.begin(); it != EventsList.end(); ++it){
			if ( input == trimName(it->getTitle())){
				matchFound = true;
				eventTitle = input;
			}
		}
		if (matchFound == false){
			cout << "Event not found. \n";
			return -1;
		}
		//===================================================================================

		//Askes the sellers name
		cout << "Which seller would you like to buy from? \n";
		cout << "Seller Name: ";
		cin >> input;
		//Check that one of the events exists with that seller==============================
		matchFound = false;
		for (vector<Event>::iterator it = EventsList.begin(); it != EventsList.end(); ++it){
			if ( eventTitle == trimName(it->getTitle()) ){
				if (input == trimName(it->getSeller() )){
					currentEvent = it;
					matchFound = true;
					sellerName = input;
				}
			}
		}
		if (matchFound == false){
			std::cout << "Could not find that seller with the named event. \n";
			return -1;
		}
		//=================================================================================
		//Checking that they do not buy more than their account allows them to / finds how many they are buying
		cout << "How many tickets would you like to buy? \n";
		cin >> NumTickets;
		if (NumTickets > buyMax){
			std::cout << "Standard accounts can only buy a max of 4 tickets at once.\n";
			return -1;
		}
		//=================================================================================
		pricePerTicket = currentEvent->getPrice(); //Finds the price for each ticket
		totalPrice = pricePerTicket * NumTickets; //Calculates the total price the buyer will have to pay
		cout << "That will be $" << pricePerTicket << " a ticket, for a total ";
		cout << "of $" << totalPrice << ". Is that okay? (y/n)\n"; 
		cin >> input;
		if (input != "yes"){
			if (totalPrice > credit){ //Make sure they have enough money to buy the tickets
				std::cout << "You do not have enough to buy these tickets. \n";
				return -1;
			}
			else if (NumTickets > currentEvent->getTicket()){ //Make sure there is that many tickets
				cout << "There are not that many tickets.\n";
				return -1;
			}
			else { //If everything runs fine
				currentEvent->subtractTicket(NumTickets); //Take away that many tickets
				credit -= totalPrice; //Charge the user
				for (vector<User>::iterator it = UsersList.begin(); it != UsersList.end(); ++it){
					if (sellerName == trimName(it->getName()) ){
						it->addCredit(totalPrice);
					}
				}
				//write to file, not implemented yet. 
				//Either writes to file here or at logout, still not sure.
				WriteEvent(EventsList);

				string ty = to_string(NumTickets);
				string cr = to_string(totalPrice);
				ty.insert(0, 3 - ty.size(), '0');
				cr = cr.substr(0, cr.size() - 4);
				cr.insert(0, 6 - cr.size(), '0');
				addToTrans("03 " + currentEvent->getTitle() + " " + sellerName.insert(sellerName.size(),14 - sellerName.size(),' ')+ " " + ty + " " + cr);

				//XX_EEEEEEEEEEEEEEEEEEEE_SSSSSSSSSSSSS_TTT_PPPPPP
				//Buy, Sell
				//03,04
				//TransactionFile << '0' << type << ' ' << event << ' ' << name << " " << tickets << ' ' << price;

				return 0;
			}
		}
	}

	return -1;
}
