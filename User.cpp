#include "User.h"
#include "Group.h"
int User::getViewsByGenre(Genre genre)
{
    if (!group)
    {
        return viewsByGenre[(int)genre];
    }
    return viewsByGenre[(int)genre] + group->getGroupWatchCounter(genre) - numOfGroupWatchWhenEntered[(int)genre];
}
void User::leaveGroup()
{
    for (int i = 0; i < 5; i++)
    {
        viewsByGenre[i] += (group->getGroupWatchCounter((Genre)i) - numOfGroupWatchWhenEntered[i]);
        numOfGroupWatchWhenEntered[i] = 0;
    }
    group = nullptr;
    prevUserInGroup = nullptr;
}