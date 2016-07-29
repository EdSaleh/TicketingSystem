#ifndef EVENT_H
#define EVENT_H

#include <string>
using namespace std;

class Event
{
private:
	string seller;
	string name;
	int tickets;
	float price;

public:
	Event() { }
	Event(string n, int t, float p);
	Event(string n, string s, int t, float p);

	void setSeller(string s);
	string getTitle();
	float getPrice();
	int getTicket();
	string getSeller();
	void subtractTicket(int s);
};

#endif