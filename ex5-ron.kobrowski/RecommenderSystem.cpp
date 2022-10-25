//
// Created by PC on 9/6/2022.
//
#include "RecommenderSystem.h"
#include <cmath>
#include <numeric>
#include <algorithm>


/*****************/
/* aux functions */
/*****************/
struct compare
{
    double num;
    compare(const double num) : num(num)
    {}
};

std::vector<double> &operator+=(std::vector<double> &v, int x)
{
    for (auto a = begin(v); a != end(v); ++a)
    {
        *a += x;
    }
    return v;
}

bool operator==(const std::pair<sp_movie, double> &p, const compare &c)
{
    return c.num == p.second;
}

bool operator==(const compare &c, const std::pair<sp_movie, double> &p)
{
    return c.num == p.second;
}

bool rs_comp(const sp_movie &mov1, const sp_movie &mov2)
{
    return *mov1 < *mov2;
}

bool rs_comp_features(const std::pair<sp_movie, std::vector<double>> &mov1,
                      const sp_movie &mov2)
{
    return rs_comp(mov1.first, mov2);
}

rank_map get_norm(const RSUser &user)
{
    auto ranks = user.get_ranks();
    auto sum = std::accumulate(ranks.begin(), ranks.end(), 0,
                               [](int value, const rank_map::value_type &p)
                               { return value + p.second; });

    auto cnt = std::count(ranks.begin(), ranks.end(),
                          NA);

    auto average = sum / (double) (ranks.size() - cnt);
    for (auto &movie: ranks)
    {
        if (movie.second != 0)
        {
            movie.second = movie.second - average;
        }
    }
    return ranks;
}

/**************/
/*   methods  */
/**************/

RecommenderSystem::RecommenderSystem() : _movie_features(rs_comp)
{};

sp_movie RecommenderSystem::add_movie(const std::string &name, int year,
                                      const std::vector<double> &features)
{
    auto spm = std::make_shared<Movie>(name, year);
    _movie_features[spm] = features;
    return spm;
}


double
RecommenderSystem::calculate_affinity(const std::vector<double> &vector_lhs,
                                      const std::vector<double> &vector_rhs) const
{
    auto up = std::inner_product(vector_rhs.begin(),
                                 vector_rhs.end(),
                                 vector_lhs.begin(), 0.0);
    auto down = sqrt(std::inner_product(vector_rhs.begin(),
                                        vector_rhs.end(),
                                        vector_rhs.begin(), 0.0)) *
                sqrt(std::inner_product(vector_lhs.begin(),
                                        vector_lhs.end(),
                                        vector_lhs.begin(), 0.0));
    if (down == 0)
    { return 0; }
    auto score = up / down;
    return score;
}

sp_movie
RecommenderSystem::choose_right_movie(const std::vector<double> &preferences,
                                      const RSUser &user) const
{
    auto best_movie = _movie_features.begin()->first;
    double best_score = 0;
    for (const auto &movie: _movie_features)
    {
        if ((user.get_ranks()[movie.first] == 0) ||
            user.get_ranks().find(movie.first) == user.get_ranks().end())
        {
            // a movie that doesn't have a rating
            double score = calculate_affinity(preferences, movie.second);
//            std::cout << score << " " << movie.first->get_name() << std::endl;
            for (auto it: movie.second)
//            { std::cout << it; }
//            std::cout << std::endl;
//            for (auto it: preferences)
//            { std::cout << it; }
//            std::cout << std::endl;
                if (score > best_score)
                {
                    best_score = score;
                    best_movie = movie.first;
                }
        }
    }
    return best_movie;
}


sp_movie RecommenderSystem::recommend_by_content(const RSUser &user)
{
    rank_map norm = get_norm(user);
    std::vector<double> preferences((_movie_features.begin()->second).size(),
                                    0.0); // new vector
    for (const auto &movie: _movie_features)
    {
        if (user.get_ranks().find(movie.first) != user.get_ranks().end())
        {
            auto pref_iter = preferences.begin();
            // entering the loop for unranked movies - average_rate_for_movie = 0
            auto average_rate_for_movie = norm[movie.first];
            for (const auto &feature: movie.second)
            {
                *pref_iter += (feature * average_rate_for_movie);
                pref_iter++;
            }
        }
    }
    return choose_right_movie(preferences, user);
}


sp_movie RecommenderSystem::recommend_by_cf(const RSUser &user, int k)
{
    auto best_score = 0.0;
    sp_movie best_movie;
    for (const auto &movie: _movie_features)
    {
        if ((user.get_ranks().find(movie.first) == user.get_ranks().end()) ||
            user.get_ranks()[movie.first] == 0)
        {
            auto score = predict_movie_score(user, movie.first, k);
            if (score > best_score)
            {
                best_score = score;
                best_movie = movie.first;
            }
        }
    }
    return best_movie;
}

double RecommenderSystem::predict_movie_score(const RSUser &user,
                                              const sp_movie &movie, int k)
{
    if(movie == nullptr){
       throw std::runtime_error(INVALID_MOVIE);
    }
    auto affinities = std::map<sp_movie, double, equal_func>(rs_comp);
    for (const auto &movie_features: _movie_features) // unseen movies
    {
        if (user.get_ranks()[movie_features.first] != NA)
        {
            auto affinity = calculate_affinity(_movie_features[movie],
                                               movie_features.second);
            affinities[movie_features.first] = affinity;
        }
    }
    int n_size = ((int) affinities.size() - k);
    for (int i = 0; i < n_size; i++)
    {
        auto min_element = std::min_element(affinities.begin(),
                                            affinities.end(),
                                            [](decltype(affinities)::value_type &l,
                                               decltype(affinities)::value_type &r) -> bool
                                            { return l.second < r.second; });
        affinities.erase(min_element);
    }
    // this point should have a k sized affinity map
    auto user_ranks = user.get_ranks();
    auto up = 0.0;
    auto down = 0.0;
    for (const auto &N_movie: affinities)
    {
        up += (N_movie.second * user_ranks[N_movie.first]);
        down += (N_movie.second);
    }
    return up / down;

}

sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
    auto spm = std::make_shared<Movie>(name, year);
//    std::pair<sp_movie , std::vector<double>> p(spm, {0,0});
//    std::binary_search(_movie_features.begin(), _movie_features.end(), p,
//                       rs_comp_features);
    auto it = _movie_features.find(spm);
    if (it == _movie_features.end())
    {
        return {nullptr};
    }
    return it->first;
}

std::ostream &operator<<(std::ostream &out, const RecommenderSystem &rs)
{
    for (std::pair<sp_movie, std::vector<double>> item: rs._movie_features)
    {
        out << item.first->get_name() << " (" << item.first->get_year() << ")";
        out << std::endl;
    }
    return out;
}
