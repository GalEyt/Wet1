#include "StreamingDBa1.h"
#include <cassert>

void unitest1_users_groups(){
    streaming_database sdb;
    sdb.add_user(1,false);
    sdb.add_user(2, true);
    sdb.add_user(3,false);
    sdb.add_user(4,false);
    sdb.add_user(5,false);
    sdb.remove_user(3);
    sdb.add_group(1);
    sdb.add_user_to_group(1,1);
    sdb.add_user_to_group(4,1);
    sdb.add_user_to_group(5,1);
    sdb.add_user_to_group(2,1);
    sdb.remove_user(5);
    sdb.remove_user(2);
    sdb.remove_user(4);
    sdb.remove_user(1);
    sdb.remove_group(1);
    StatusType result = sdb.add_user_to_group(1,1);
    sdb.add_group(1);
    result = sdb.add_user_to_group(6,1);
    result = sdb.add_user_to_group(3,1);
    result = sdb.remove_user(9);
    result = sdb.remove_group(8);
    result = sdb.add_user(-1, false);
    result = sdb.add_group(1);
    result = sdb.add_user(1, true);
}

void unitest2_movies(){
    streaming_database sdb;
    StatusType result;
    for (int i = 1; i < 5; i++)
    {
        assert(sdb.add_movie(i,Genre::ACTION, i-1, !(bool)(i-1)) == StatusType::SUCCESS);
        assert(sdb.add_movie(i+5,Genre::COMEDY, i+4, !(bool)(i-1))== StatusType::SUCCESS);
        assert(sdb.add_movie(i+10,Genre::FANTASY, i+6, !(bool)(i-1))== StatusType::SUCCESS);
        assert(sdb.add_movie(i+20,Genre::DRAMA, i+20, !(bool)(i-1))== StatusType::SUCCESS);
    }
    assert(sdb.add_movie(100, Genre::ACTION, 0, false)== StatusType::SUCCESS);
    assert(sdb.add_movie(101, Genre::ACTION, 0, true)== StatusType::SUCCESS);
    assert(sdb.add_user(1,false)== StatusType::SUCCESS);
    assert(sdb.add_user(2, true)== StatusType::SUCCESS);
    assert(sdb.add_user(3,false)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 101)== StatusType::FAILURE);
    assert(sdb.user_watch(2, 101)== StatusType::SUCCESS);
    
}



int main()
{
    unitest2_movies();
    return 0;
}