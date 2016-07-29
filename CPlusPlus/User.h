#ifndef USER_H
#define USER_H

#include <string>
#include "Event.h"
using namespace std;

class User
{
protected:
	string name;
	int type; //01 Admin, 02 Buy, 03 Sell, 04 Full Standard
	float credit;
	int buyMax;

public:
	User(){ };
	User(string n, int t, float c);

	string getName();
	int getType();
	float getCredit();
	int addCredit(float a);
	int sell(vector<User> UsersList, vector<Event> EventsList);
	int buy(vector<User> UsersList, vector<Event> EventsList);
};

#endif