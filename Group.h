#ifndef GROUP
#define GROUP
#include "wet1util.h"
#include "User.h"
#include "Node.h"
#include <memory>
class Group
{
	int groupID = 0;
	bool vip = false;
	int viewsOfUsersByGenre[5] = {0, 0, 0, 0, 0};
	int viewsAsGroup[5] = {0, 0, 0, 0, 0};
	int groupWatchCounter[5] = {0, 0, 0, 0, 0};
	int numOfUsers = 0;
	int numOfVip = 0;
	Node<std::shared_ptr<User>> *users = nullptr;
	Node<std::shared_ptr<User>> *lastUser = nullptr;
	void addViewsOfUser(std::shared_ptr<User> user)
	{
		for (int i = 0; i < 5; i++)
		{
			viewsOfUsersByGenre[i] += user->getViewsByGenre((Genre)i);
		}
	}
	void removeViewsOfUser(std::shared_ptr<User> user)
	{
		for (int i = 0; i < 5; i++)
		{
			viewsOfUsersByGenre[i] -= user->getViewsByGenreForRemove((Genre)i);
			viewsAsGroup[i] -= (groupWatchCounter[i] - user->getViewsOfGroupWhenEntered(i));
		}
	}

public:
	Group(int id) : groupID(id)
	{
		users = new Node<std::shared_ptr<User>>(std::shared_ptr<User>(new User(-1, false)));
		users->data->setGroup(this);
		lastUser = users;
	}
	~Group()
	{
		Node<std::shared_ptr<User>> *tmp = users;
		Node<std::shared_ptr<User>> *toDelete;
		while (tmp)
		{
			toDelete = tmp;
			tmp = tmp->next;
			toDelete->data->leaveGroup();
			delete toDelete;
			toDelete = nullptr;
		}
		users = nullptr;
		lastUser = nullptr;
	}
	bool isVIP() { return vip; }
	int getViewsByGenre(Genre genre)
	{
		return viewsOfUsersByGenre[(int)genre] + viewsAsGroup[(int)genre];
	}
	int getNumOfUsers() { return numOfUsers; }
	int getID() { return groupID; }
	void addUser(std::shared_ptr<User> user)
	{
		user->setPrevUserInGroup(lastUser);
		lastUser->next = new Node<std::shared_ptr<User>>(user);
		lastUser = lastUser->next;
		numOfUsers++;
		addViewsOfUser(user);
		for (int i = 0; i < 5; i++)
		{
			user->setViewsOfGroupWhenEntered(i, groupWatchCounter[i]);
		}
		
		if (user->isVIP())
		{
			vip = true;
			numOfVip++;
		}
		user->setGroup(this);
	}

	void removeUser(std::shared_ptr<User> user)
	{
		numOfUsers--;
		if (user->isVIP())
		{
			numOfVip--;
			if (numOfVip == 0)
			{
				vip = false;
			}
		}
		removeViewsOfUser(user);
		if (lastUser->data->getID() == user->getID())
		{
			lastUser = user->getPrevUserInGroup();
		}
		Node<std::shared_ptr<User>> *toDelete = user->getPrevUserInGroup()->next;
		user->getPrevUserInGroup()->next = toDelete->next;
		if (toDelete->next)
		{
			toDelete->next->data->setPrevUserInGroup(toDelete->data->getPrevUserInGroup());
		}
		user->leaveGroup();
		delete toDelete;
	}

	void addView(Genre genre)
	{
		groupWatchCounter[(int)genre]++;
		groupWatchCounter[(int)Genre::NONE]++;
		viewsAsGroup[(int)genre] += numOfUsers;
		viewsAsGroup[(int)Genre::NONE] += numOfUsers;
	}

	void userWatchedAlone(Genre genre){
		viewsOfUsersByGenre[(int)genre]++;
		viewsOfUsersByGenre[(int)Genre::NONE]++;
	}

	int getGroupWatchCounter(Genre genre)
	{
		return groupWatchCounter[(int)genre];
	}

	Genre getFavouriteGenre()
	{
		int max = getViewsByGenre((Genre)0);
		Genre maxGenre = (Genre)0;
		for (int i = 1; i < 4; i++)
		{
			if (max < getViewsByGenre((Genre)i))
			{
				max = getViewsByGenre((Genre)i);
				maxGenre = (Genre)i;
			}
		}
		return maxGenre;
	}
};

#endif // GROUP