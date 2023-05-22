#include "StreamingDBa1.h"
#include <exception>

streaming_database::streaming_database() : users(new AVLTree<std::shared_ptr<User>, int>),
										   groups(new AVLTree<std::shared_ptr<Group>, int>),
										   moviesByID(new AVLTree<std::shared_ptr<Movie>, int>),
										   moviesByGrade(new AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>),
										   actionByGrade(new AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>),
										   dramaByGrade(new AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>),
										   comedyByGrade(new AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>),
										   fantasyByGrade(new AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>>)

{
}

streaming_database::~streaming_database()
{
	groups->deleteTree(groups);
	users->deleteTree(users);
	moviesByGrade->deleteTree(moviesByGrade);
	actionByGrade->deleteTree(actionByGrade);
	dramaByGrade->deleteTree(dramaByGrade);
	comedyByGrade->deleteTree(comedyByGrade);
	fantasyByGrade->deleteTree(fantasyByGrade);
	moviesByID->deleteTree(moviesByID);
}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	if (movieId <= 0 || genre == Genre::NONE || views < 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<Movie> movie(new Movie(movieId, genre, views, vipOnly));
		moviesByID = moviesByID->insert(movie, movieId);
		addMovieByGenre(movie);
		favouriteMoviesByGenre[(int)genre] = getBestMovie(genre);
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Exist)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
	if (movieId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<Movie> movie(moviesByID->find(movieId)->getData());
		moviesByID = moviesByID->remove(movieId);
		removeMovieByGenre(movie);
		favouriteMoviesByGenre[(int)movie->getGenre()] = getBestMovie(movie->getGenre());
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
	if (userId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(new User(userId, isVip));
		users = users->insert(user, userId);
		numOfUsers++;
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Exist)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
	if (userId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(users->find(userId)->getData());
		if (user->getGroup())
		{
			user->getGroup()->removeUser(user);
		}
		users = users->remove(userId);
		numOfUsers--;
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
	if (groupId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<Group> group(new Group(groupId));
		groups = groups->insert(group, groupId);
		numOfGroups++;
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Exist)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
	if (groupId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<Group> group(groups->find(groupId)->getData());
		groups = groups->remove(groupId);
		numOfGroups--;
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	if (userId <= 0 || groupId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(users->find(userId)->getData());
		std::shared_ptr<Group> group(groups->find(groupId)->getData());
		if (user->getGroup())
		{
			return StatusType::FAILURE;
		}
		group->addUser(user);
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
	if (userId <= 0 || movieId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(users->find(userId)->getData());
		std::shared_ptr<Movie> movie(moviesByID->find(movieId)->getData());
		if (!user->isVIP() && movie->isVIP())
		{
			return StatusType::FAILURE;
		}
		updateMoviePosition(movie, 1, -1);
		favouriteMoviesByGenre[(int)movie->getGenre()] = getBestMovie(movie->getGenre());
		viewsByGenre[(int)movie->getGenre()]++;
		viewsByGenre[(int)Genre::NONE]++;
		user->addView(movie->getGenre());
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId, int movieId)
{
	if (groupId <= 0 || movieId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<Group> group(groups->find(groupId)->getData());
		std::shared_ptr<Movie> movie(moviesByID->find(movieId)->getData());
		if (!group->isVIP() && movie->isVIP())
		{
			return StatusType::FAILURE;
		}
		if(group->getNumOfUsers() == 0){
			return StatusType::FAILURE;
		}
		updateMoviePosition(movie, group->getNumOfUsers(), -1);
		favouriteMoviesByGenre[(int)movie->getGenre()] = getBestMovie(movie->getGenre());
		viewsByGenre[(int)movie->getGenre()] += group->getNumOfUsers();
		viewsByGenre[(int)Genre::NONE] += group->getNumOfUsers();
		group->addView(movie->getGenre());
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
	return output_t<int>(numOfMoviesByGenre[(int)genre]);
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	if (!output)
	{
		return StatusType::INVALID_INPUT;
	}
	if (numOfMoviesByGenre[(int)genre] == 0)
	{
		return StatusType::FAILURE;
	}
	switch (genre)
	{
	case Genre::ACTION:
		getAllMoviesHelper(actionByGrade, output, 0);
		break;
	case Genre::COMEDY:
		getAllMoviesHelper(comedyByGrade, output, 0);
		break;
	case Genre::DRAMA:
		getAllMoviesHelper(dramaByGrade, output, 0);
		break;
	case Genre::FANTASY:
		getAllMoviesHelper(fantasyByGrade, output, 0);
		break;
	default:
		getAllMoviesHelper(moviesByGrade, output, 0);
		break;
	}

	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	if (userId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(users->find(userId)->getData());
		return output_t<int>(user->getViewsByGenre(genre));
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
	if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
	{
		return StatusType::INVALID_INPUT;
	}
	try
	{
		std::shared_ptr<User> user(users->find(userId)->getData());
		std::shared_ptr<Movie> movie(moviesByID->find(movieId)->getData());
		if (!user->isVIP() && movie->isVIP())
		{
			return StatusType::FAILURE;
		}
		updateMoviePosition(movie, 0, rating);
		favouriteMoviesByGenre[(int)movie->getGenre()] = getBestMovie(movie->getGenre());
	}
	catch (std::bad_alloc)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (NotFound)
	{
		return StatusType::FAILURE;
	}
	catch (EmptyTree)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0)
	{
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	try
	{
		std::shared_ptr<Group> group(groups->find(groupId)->getData());
		if (group->getNumOfUsers() == 0)
		{
			return output_t<int>(StatusType::FAILURE);
		}
		Genre favGen = group->getFavouriteGenre();
		int bestID = favouriteMoviesByGenre[(int)favGen]->getID();
		if (bestID == -1)
		{
			return output_t<int>(StatusType::FAILURE);
		}
		return output_t<int>(bestID);
	}
	catch (std::bad_alloc)
	{
		return output_t<int>(StatusType::ALLOCATION_ERROR);
	}
	catch (NotFound)
	{
		return output_t<int>(StatusType::FAILURE);
	}
	catch (EmptyTree)
	{
		return output_t<int>(StatusType::FAILURE);
	}
	return StatusType::SUCCESS;
}

void streaming_database::addMovieByGenre(std::shared_ptr<Movie> movie)
{
	Genre genre = movie->getGenre();
	switch (genre)
	{
	case Genre::ACTION:
		actionByGrade = actionByGrade->insert(movie, movie);
		break;
	case Genre::COMEDY:
		comedyByGrade = comedyByGrade->insert(movie, movie);
		break;
	case Genre::DRAMA:
		dramaByGrade = dramaByGrade->insert(movie, movie);
		break;
	case Genre::FANTASY:
		fantasyByGrade = fantasyByGrade->insert(movie, movie);
		break;
	}
	moviesByGrade = moviesByGrade->insert(movie, movie);
	viewsByGenre[(int)genre] += movie->getViews();
	viewsByGenre[(int)Genre::NONE] += movie->getViews();
	numOfMoviesByGenre[(int)genre]++;
	numOfMoviesByGenre[(int)Genre::NONE]++;
}

void streaming_database::removeMovieByGenre(std::shared_ptr<Movie> movie)
{
	Genre genre = movie->getGenre();
	switch (genre)
	{
	case Genre::ACTION:
		actionByGrade = actionByGrade->remove(movie);
		break;
	case Genre::COMEDY:
		comedyByGrade = comedyByGrade->remove(movie);
		break;
	case Genre::DRAMA:
		dramaByGrade = dramaByGrade->remove(movie);
		break;
	case Genre::FANTASY:
		fantasyByGrade = fantasyByGrade->remove(movie);
		break;
	}
	moviesByGrade = moviesByGrade->remove(movie);
	viewsByGenre[(int)genre] -= movie->getViews();
	viewsByGenre[(int)Genre::NONE] -= movie->getViews();
	numOfMoviesByGenre[(int)genre]--;
	numOfMoviesByGenre[(int)Genre::NONE]--;
}

int streaming_database::getAllMoviesHelper(AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>> *node, int *const output, int i)
{

	if (node->getRight())
	{
		i = getAllMoviesHelper(node->getRight(), output, i);
	}
	output[i] = node->getData()->getID();
	i++;
	if (node->getLeft())
	{
		i = getAllMoviesHelper(node->getLeft(), output, i);
	}
	return i;
}

void streaming_database::updateMovieHelper(std::shared_ptr<Movie> movie, int views, int rating)
{
	movie->addViews(views);
	if (rating >= 0)
	{
		movie->addReview(rating);
	}
}

void streaming_database::updateMoviePosition(std::shared_ptr<Movie> movie, int views, int rating)
{
	Genre genre = movie->getGenre();
	moviesByGrade = moviesByGrade->remove(movie);
	switch (genre)
	{
	case Genre::ACTION:
	{
		actionByGrade = actionByGrade->remove(movie);
		updateMovieHelper(movie, views, rating);
		actionByGrade = actionByGrade->insert(movie, movie);
		break;
	}
	case Genre::COMEDY:
		comedyByGrade = comedyByGrade->remove(movie);
		updateMovieHelper(movie, views, rating);
		comedyByGrade = comedyByGrade->insert(movie, movie);
		break;
	case Genre::DRAMA:
		dramaByGrade = dramaByGrade->remove(movie);
		updateMovieHelper(movie, views, rating);
		dramaByGrade = dramaByGrade->insert(movie, movie);
		break;
	case Genre::FANTASY:
		fantasyByGrade = fantasyByGrade->remove(movie);
		updateMovieHelper(movie, views, rating);
		fantasyByGrade = fantasyByGrade->insert(movie, movie);
		break;
	}
	moviesByGrade = moviesByGrade->insert(movie, movie);
}

std::shared_ptr<Movie> getMostRight(AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>> *root)
{
	if (root->getHeight() == -1)
	{
		return std::shared_ptr<Movie>(new Movie());
	}
	AVLTree<std::shared_ptr<Movie>, std::shared_ptr<Movie>> *tmp = root;
	while (tmp->getRight())
	{
		tmp = tmp->getRight();
	}
	return tmp->getID();
}

std::shared_ptr<Movie> streaming_database::getBestMovie(Genre genre)
{
	switch (genre)
	{
	case Genre::ACTION:
		return getMostRight(actionByGrade);
	case Genre::COMEDY:
		return getMostRight(comedyByGrade);
	case Genre::DRAMA:
		return getMostRight(dramaByGrade);
	case Genre::FANTASY:
		return getMostRight(fantasyByGrade);
	default:
		return std::shared_ptr<Movie>(new Movie());
	}
}
