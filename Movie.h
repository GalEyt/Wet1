#ifndef MOVIE
#define MOVIE
#include "wet1util.h"
class Movie
{
    int movieID;
    Genre genre;
    int views;
    int sumOfReviews;
    int numOfReviews;
    bool vip;

public:
    Movie(Genre gnr, bool vip) : genre(gnr), vip(vip) {}
    Genre getGenre() const { return genre; }
    int getViews() const { return views; }
    bool isVIP() const { return vip; }
    int getID() const{ return movieID;}
    double getAverageGrade() const
    {
        if (numOfReviews)
        {
            return 0;
        }
        return (double)sumOfReviews / (double)numOfReviews;
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
bool operator<(const Movie &movie1, const Movie &movie2)
{
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
        return false;
    }
    if (movie1.getViews() > movie2.getViews())
    {
        return true;
    }
    return movie1.getID() < movie2.getID();
}
#endif // MOVIE