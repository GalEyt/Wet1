#ifndef MOVIE
#define MOVIE
#include "wet1util.h"
#include <memory>
class Movie
{
    int movieID;
    Genre genre;
    int views = 0;
    int sumOfReviews = 0;
    int numOfReviews = 0;
    bool vip;

public:
    explicit Movie() : movieID(-1), genre((Genre)0), views(0), vip(false) {}
    explicit Movie(int id, Genre gnr, int views, bool vip) : movieID(id), genre(gnr), views(views), vip(vip) {}
    Genre getGenre() const { return genre; }
    int getViews() const { return views; }
    bool isVIP() const { return vip; }
    int getID() const{ return movieID;}
    double getAverageGrade() const
    {
        if (numOfReviews == 0)
        {
            return 0;
        }
        double result = sumOfReviews / numOfReviews;
        return result;
    }
    void addViews(int numOfViews)
    {
        views += numOfViews;
    }
    void addReview(int grade)
    {
        numOfReviews++;
        sumOfReviews += grade;
    }
};
inline bool operator<(const Movie &movie1, const Movie &movie2)
{
    double grade1 = movie1.getAverageGrade();
    double grade2 =  movie2.getAverageGrade();
    if (movie1.getAverageGrade() < movie2.getAverageGrade())
    {
        return true;
    }
    if (movie1.getAverageGrade() > movie2.getAverageGrade())
    {
        return false;
    }
    if (movie1.getViews() < movie2.getViews())
    {
        return true;
    }
    if (movie1.getViews() > movie2.getViews())
    {
        return false;
    }
    return movie1.getID() > movie2.getID();
}

inline bool operator==(const Movie &movie1, const Movie &movie2) {
    return !(movie1 < movie2 || movie2 < movie1);
}

inline bool operator<(std::shared_ptr<Movie> movie1, std::shared_ptr<Movie> movie2)
{
    return *movie1 < *movie2;
}

inline bool operator==(std::shared_ptr<Movie> movie1, std::shared_ptr<Movie> movie2) {
    return *movie1 == *movie2;
}

#endif // MOVIE