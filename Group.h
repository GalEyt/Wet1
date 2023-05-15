#ifndef GROUP
#define GROUP
#include "wet1util.h"
#include "User.h"
#include "Node.h"
class Group
{
	int groupID;
	bool vip = false;
	int viewsByGenre[5]; // num of genres
	int numOfUsers;
	Node<User *> *users = nullptr;
	void addViewsOfUser(User *user)
	{
		for (int i = 0; i < 5; i++)
		{
			viewsByGenre[i] += user->getViewsByGenre((Genre)i);
		}
	}

public:
	Group(int id) : groupID(id){}
	bool isVIP() { return vip; }
	int getViewsByGenre(Genre genre) { return viewsByGenre[(int)genre]; }
	int getNumOfUsers() { return numOfUsers; }
	int getID(){return groupID;}
	StatusType addUser(User *user)
	{
		if (!user)
		{
			return StatusType::INVALID_INPUT;
		}
		if (!users)
		{
			users = new Node<User *>(user);
		}
		else
		{
			Node<User *> *itr = users;
			while (itr->next)
			{
				if (user->getID() == itr->data->getID())
				{
					return StatusType::FAILURE;
				}
				itr = itr->next;
			}
			itr->next = new Node<User *>(user);
		}
		numOfUsers++;
		addViewsOfUser(user);
		if(user->isVIP()){
			vip = true;
		}
		return StatusType::SUCCESS;
	}
};

#endif // GROUP