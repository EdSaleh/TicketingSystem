#ifndef ADMIN_H
#define ADMIN_H

#include <string>
using namespace std; 

class Admin : public User
{
public:
	Admin(){ };
	Admin(string n, float c);
	Admin(User u);
	int create(vector<User> UsersList);
	int del(vector<User> UsersList, vector<Event> EventsList);
	int refund(vector<User> UsersList);
	int addCredit(vector<User> UsersList);
	
};

#endif