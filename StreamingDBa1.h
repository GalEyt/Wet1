// 
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Group.h"
#include "Movie.h"
#include <memory>

class streaming_database {
private:
	//
	// Here you may add anything you want
	//
	int numOfUsers = 0;
	int numOfGroups = 0;
	int viewsByGenre[5] = {0,0,0,0,0};
	int numOfMoviesByGenre[5]= {0,0,0,0,0};
	AVLTree<std::shared_ptr<User>, int>* users;
	AVLTree<std::shared_ptr<Group>, int>* groups;
	AVLTree<std::shared_ptr<Movie>, int>* moviesByID;
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* moviesByGrade;
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* actionByGrade;
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* dramaByGrade;
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* comedyByGrade;
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* fantasyByGrade;
	std::shared_ptr<Movie> favouriteMoviesByGenre[4];
	void addMovieByGenre(std::shared_ptr<Movie>);
	void removeMovieByGenre(std::shared_ptr<Movie>);
	int getAllMoviesHelper(AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>* node, int* const output, int i);
	void updateMoviePosition(std::shared_ptr<Movie>, int);
	std::shared_ptr<Movie> getBestMovie(Genre genre);

public:
	// <DO-NOT-MODIFY> {
	
	streaming_database();

	virtual ~streaming_database();
	
	StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);
	
	StatusType remove_movie(int movieId);
	
	StatusType add_user(int userId, bool isVip);
	
	StatusType remove_user(int userId);
	
	StatusType add_group(int groupId);
	
	StatusType remove_group(int groupId);

	StatusType add_user_to_group(int userId, int groupId);
	
	StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
