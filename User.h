#ifndef USER
#define USER
#include "wet1util.h"
#include "Node.h"
#include <memory>
class Group;
class User
{
	int id;
	bool vip;
	Group *group = nullptr;
	int numOfGroupWatchWhenEntered[5]= {0, 0, 0, 0, 0};
	Node<std::shared_ptr<User>> *prevUserInGroup = nullptr;
	int viewsByGenre[5] = {0, 0, 0, 0, 0}; // num of genres

public:
	User(int id, bool vip) : id(id), vip(vip) {}
	bool isVIP() { return vip; }
	Group *getGroup() { return group; }
	int getID() { return id; }
	int getViewsOfGroupWhenEntered(int index)
	{
		return numOfGroupWatchWhenEntered[index];
	}
	int getViewsByGenreForRemove(Genre genre);
	void setViewsOfGroupWhenEntered(int index, int views){
		numOfGroupWatchWhenEntered[index] = views;
	}
	Node<std::shared_ptr<User>> *getPrevUserInGroup()
	{
		return prevUserInGroup;
	}
	void setPrevUserInGroup(Node<std::shared_ptr<User>> *prevUser)
	{
		prevUserInGroup = prevUser;
	}
	int getViewsByGenre(Genre genre);
	void setGroup(Group *newGroup)
	{
		group = newGroup;
	}
	void addView(Genre genre);
	
	void leaveGroup();
};
#endif // USER