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
    int num_movies = sdb.get_all_movies_count(Genre::NONE).ans();
    int num_moviesAction = sdb.get_all_movies_count(Genre::ACTION).ans();
    int* arr = new int[num_movies];
    int* arrAction = new int[num_moviesAction];
    assert(sdb.rate_movie(1, 100, 100)== StatusType::SUCCESS);
    assert(sdb.get_all_movies(Genre::NONE, arr)== StatusType::SUCCESS);
    for (int i = 0; i < num_movies; i++)
    {
        std::cout << "id: " << arr[i] << "\n";
    }
    std::cout << "-----------------------\n";
    assert(sdb.get_all_movies(Genre::ACTION, arrAction)== StatusType::SUCCESS);
    for (int i = 0; i < num_moviesAction; i++)
    {
        std::cout << "id: " << arrAction[i] << "\n";
    }
    
    for (int i = 1; i < 5; i++)
    {
        assert(sdb.remove_movie(i) == StatusType::SUCCESS);
        assert(sdb.remove_movie(i+5)== StatusType::SUCCESS);
        assert(sdb.remove_movie(i+10)== StatusType::SUCCESS);
        assert(sdb.remove_movie(i+20)== StatusType::SUCCESS);
    }
    assert(sdb.rate_movie(1, 100, 5)== StatusType::SUCCESS);
    assert(sdb.rate_movie(1, 101, 10)== StatusType::FAILURE);
    assert(sdb.rate_movie(2, 100, 5)== StatusType::SUCCESS);
    assert(sdb.rate_movie(2, 101, 10)== StatusType::SUCCESS);
    assert(sdb.rate_movie(3, 100, -1)== StatusType::INVALID_INPUT);
    assert(sdb.rate_movie(3, 101, 10)== StatusType::FAILURE);
    assert(sdb.remove_movie(100)== StatusType::SUCCESS);
    assert(sdb.remove_movie(101)== StatusType::SUCCESS);
    delete[] arr;
    delete[] arrAction;

    std::cout <<"yo";
}

void unitest_recommend_views(){
    streaming_database sdb;
    assert(sdb.add_user(1,false)== StatusType::SUCCESS);
    assert(sdb.add_user(2, true)== StatusType::SUCCESS);
    assert(sdb.add_group(1)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(1, 1)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(2, 1)== StatusType::SUCCESS);
    assert(sdb.add_group(2)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(1, 1)== StatusType::FAILURE);
    assert(sdb.add_user_to_group(1, 2)== StatusType::FAILURE);
    assert(sdb.add_movie(100, Genre::ACTION, 0, false)== StatusType::SUCCESS);
    assert(sdb.add_movie(101, Genre::ACTION, 0, true)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.user_watch(2, 101)== StatusType::SUCCESS);
    assert(sdb.group_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.group_watch(1, 101)== StatusType::SUCCESS);
    assert(sdb.group_watch(2, 100)== StatusType::FAILURE);
    assert(sdb.add_user(3, false)== StatusType::SUCCESS);
    assert(sdb.user_watch(3, 100)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(3, 1)== StatusType::SUCCESS);
    assert(sdb.group_watch(1, 101)== StatusType::SUCCESS);
    assert(sdb.get_num_views(1, Genre::NONE).ans() == 6);
    assert(sdb.get_num_views(2, Genre::NONE).ans() == 4);
    assert(sdb.get_num_views(3, Genre::NONE).ans() == 2);
    assert(sdb.get_group_recommendation(1).ans() == 100);
    assert(sdb.rate_movie(2, 101, 50)== StatusType::SUCCESS);
    assert(sdb.get_group_recommendation(1).ans() == 101);
    assert(sdb.rate_movie(2, 100, 50)== StatusType::SUCCESS);
    assert(sdb.remove_user(1)== StatusType::SUCCESS);
    assert(sdb.get_group_recommendation(1).ans() == 100);
    assert(sdb.get_num_views(2, Genre::NONE).ans() == 4);
    assert(sdb.get_num_views(3, Genre::NONE).ans() == 2);
}

/*
int main()
{
    unitest_recommend_views();
    std::cout << "success";
    return 0;
}*/