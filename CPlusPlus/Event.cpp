#include <string>
#include "Event.h"
using namespace std;

//Function: Event
//Input: String n, float p, int t
//Called from Transaction
Event::Event(string n,int t, float p){ //Initializing
	name = n;
	price = p;
	tickets = t;
}
//Function: Event
//Input: String n, String s, float p, int t
//Called From Main
Event::Event(string n, string s, int t, float p){ //Initializing
	name = n;
	seller = s;
	price = p;
	tickets = t;
}

//Set Function
void Event::setSeller(string s){	//Setting the seller.
	seller = s;
}

//Get Functions================================================================
string Event::getTitle(){		//Returns the event title
	return name;
}
float Event::getPrice(){		//Returns the price 
	return price;
}
int Event::getTicket(){		//Returns the amount of tickets available
	return tickets;
}
string Event::getSeller(){		//Returns the Seller's User Name
	return seller;
}
//===============================================================================

//Function: subtractTickets
//Subtracts the inputed number from the number of available tickets
//Input: int s
//Called By: User
void Event::subtractTicket(int s){	//Subtract tickets for when some have been bought
	tickets -= s;
}
