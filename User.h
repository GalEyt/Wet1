#ifndef USER
#define USER
#include "wet1util.h"
#include "Group.h"
class User{
	int id;
	bool vip;
	Group* group;
	int viewsByGenre[5]; // num of genres

public:
	User(bool vip) : vip(vip) {}
	bool isVIP() { return vip; }
	Group* getGroup() { return group; }
	int getID() { return id;}
	int getViewsByGenre(Genre genre) { return viewsByGenre[(int)genre]; }
	void setGroup(Group* newGroup) {
		group = newGroup;
	}
	bool addView(Genre genre) {
		if (genre == Genre::NONE) {
			return false;
		}
		viewsByGenre[(int)genre]++;
		viewsByGenre[(int)Genre::NONE]++;
		return true;
	}
};
#endif // USER