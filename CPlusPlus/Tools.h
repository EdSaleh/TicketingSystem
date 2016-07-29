#include <string>
#include "User.h"
using namespace std;

string trimName(string addedSpace);
vector<User> UpdateUsers();
vector<Event> UpdateEvents();
void WriteUsers(vector<User> u);
void WriteEvent(vector<Event> e);
void addToTrans(string s);
string getTrans();