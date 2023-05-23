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

void valg(){
    streaming_database sdb;
sdb.add_group(6567);
sdb.add_user_to_group( 3347, 8466);
sdb.add_group(7703);
sdb.add_user( 9744 , false);
sdb.add_user_to_group (9744, 6567);
sdb.add_group(6727);
sdb.rate_movie( 9744, 5173, 2);
sdb.group_watch( 7703, 6945);
sdb.group_watch( 6727, 264);
sdb.remove_user (9744);
sdb.rate_movie (6756, 4670, 116);
sdb.add_user (8287, false);
sdb.add_group(9143);
}

void recomTest(){
    streaming_database sdb;
    assert(sdb.add_group(3906)== StatusType::SUCCESS);
    assert(sdb.add_user(6174,false)== StatusType::SUCCESS);
    assert(sdb.add_user(5740, false)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(6174, 3906)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(5740, 3906)== StatusType::SUCCESS);
    assert(sdb.rate_movie(5740, 23525, 14)== StatusType::FAILURE);
    assert(sdb.get_num_views(5740,(Genre)2).ans() == 0);
    assert(sdb.remove_user(5740)== StatusType::SUCCESS);
    assert(sdb.add_movie(1376, (Genre)3, 8, false)== StatusType::SUCCESS);
    assert(sdb.add_movie(6549, (Genre)2, 58, false)== StatusType::SUCCESS);
    assert(sdb.add_user(1211,false)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(1211, 3906)== StatusType::SUCCESS);
    assert(sdb.user_watch(1211, 1376)== StatusType::SUCCESS);
    assert(sdb.rate_movie(1211, 6549, 14)== StatusType::SUCCESS);
    assert(sdb.get_group_recommendation(3906).ans() == 1376);
}

void recomTest2(){
    streaming_database sdb;
    assert(sdb.add_movie(5845, (Genre)1, 87, true)== StatusType::SUCCESS); //check
    assert(sdb.add_movie(1523, (Genre)3, 84, false)== StatusType::SUCCESS);
    assert(sdb.add_movie(3526, (Genre)0, 29, true)== StatusType::SUCCESS);
    assert(sdb.add_movie(4133, (Genre)0, 99, true)== StatusType::SUCCESS);
    assert(sdb.user_watch(5380, 4133)== StatusType::FAILURE);
    assert(sdb.add_movie(8182, (Genre)2, 23, false)== StatusType::SUCCESS);
    assert(sdb.add_movie(4982, (Genre)2, 53, false)== StatusType::SUCCESS);
    assert(sdb.add_group(2567)== StatusType::SUCCESS);
    assert(sdb.add_user(1809,false)== StatusType::SUCCESS);
    assert(sdb.user_watch(1809, 4982)== StatusType::SUCCESS);
    assert(sdb.remove_movie(3526)== StatusType::SUCCESS);
    assert(sdb.add_movie(8884, (Genre)0, 36, false)== StatusType::SUCCESS);
    assert(sdb.add_user(471,false)== StatusType::SUCCESS);
    assert(sdb.group_watch(2567, 8182)== StatusType::FAILURE);
    assert(sdb.add_movie(7391, (Genre)0, 42, true)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(1809, 2567)== StatusType::SUCCESS);
    assert(sdb.add_user(1425,true)== StatusType::SUCCESS);
    assert(sdb.group_watch(2567, 8884)== StatusType::SUCCESS);    
    assert(sdb.add_user_to_group(1425, 2567)== StatusType::SUCCESS);
    assert(sdb.add_user(8274, false)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(8274, 2567)== StatusType::SUCCESS);
    assert(sdb.rate_movie(471, 4982, 84)== StatusType::SUCCESS);
    assert(sdb.user_watch(8274, 4982)== StatusType::SUCCESS);
    assert(sdb.get_group_recommendation(2567).ans() == 4982);

}

void recomTest3(){
    streaming_database sdb;
    assert(sdb.add_user(1,false)== StatusType::SUCCESS);
    assert(sdb.add_user(2, true)== StatusType::SUCCESS);
    assert(sdb.add_group(1)== StatusType::SUCCESS);
    assert(sdb.add_movie(100, Genre::ACTION, 0, false)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.user_watch(2, 100)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(1, 1)== StatusType::SUCCESS);
    assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(2, 1)== StatusType::SUCCESS);
    assert(sdb.group_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.remove_user(1)== StatusType::SUCCESS);
    assert(sdb.group_watch(1, 100)== StatusType::SUCCESS);

    assert(sdb.remove_group(1)== StatusType::SUCCESS);
    //assert(sdb.user_watch(1, 100)== StatusType::SUCCESS);
    assert(sdb.add_group(2)== StatusType::SUCCESS);
    assert(sdb.add_user_to_group(2, 2)== StatusType::SUCCESS);
   // assert(sdb.add_user_to_group(1, 2)== StatusType::SUCCESS);

}

void recomTest4(){
    streaming_database sdb;
    assert(sdb.add_user(1,false)== StatusType::SUCCESS);
    assert(sdb.add_movie(100, Genre::ACTION, 0, false)== StatusType::SUCCESS);

}

/*int main()
{
    recomTest4();
    std::cout << "success";
    return 0;
}*/