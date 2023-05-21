#ifndef MOVIE
#define MOVIE
#include "wet1util.h"
class Movie
{
    int movieID;
    Genre genre;
    int views = 0;
    int sumOfReviews = 0;
    int numOfReviews = 0;
    bool vip;

public:
    Movie() : movieID(-1), genre((Genre)0), views(0), vip(false) {}
    Movie(int id, Genre gnr, int views, bool vip) : movieID(id), genre(gnr), views(views), vip(vip) {}
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
inline bool operator<(const Movie &movie1, const Movie &movie2)
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

inline bool operator==(const Movie &movie1, const Movie &movie2) {
    return !(movie1 < movie2 || movie2 < movie1);
}

#endif // MOVIE