//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include <cmath>
#include <utility>
#include "RecommenderSystem.h"
#define MIN 1
#define MAX 10


// implement your cpp code here
RSUser::RSUser(std::string &name, rank_map rankings, std::shared_ptr<RecommenderSystem> rsp)
{
    _name = std::move(name);
    _rankings = std::move(rankings);
    _rsp = std::move(rsp);
}

std::string RSUser::get_name() const
{
    return _name;
}

void RSUser::add_movie_to_rs(const std::string &name, int year,
                             const std::vector<double> &features, double rate)
{
    if(rate<MIN || rate > MAX){
        throw std::runtime_error(INVALID_MOVIE);
    }

    _rsp->add_movie(name, year, features);
    sp_movie sp = std::make_shared<Movie>(name, year);
    if(rate <= MAX || rate >= MIN){
        _rankings[sp] = rate;
    }
    else{_rankings[sp] = NA;}
}

rank_map RSUser::get_ranks() const
{
    // is this returns a copy?
    return _rankings;
}


sp_movie RSUser::get_recommendation_by_content() const
{
    return _rsp->recommend_by_content(*this);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
    return _rsp->recommend_by_cf(*this, k);

}

std::ostream &operator<<(std::ostream &out, const RSUser& user)
{
    out << "name: " << user._name;
    for (std::pair<sp_movie, double> i: user._rankings){
        out << i.first;
    }
    out << std::endl; // might need to be deleted
    return out;
}

double
RSUser::get_prediction_score_for_movie(const std::string &name, int year,
                                       int k) const
{
    return _rsp->predict_movie_score(*this, _rsp->get_movie(name, year), k);
}





