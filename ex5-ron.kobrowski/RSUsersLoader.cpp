//
// Created by PC on 9/6/2022.
//
#include "RSUsersLoader.h"
#include <fstream>
#include <sstream>
#include <cmath>
#define WORDS_DELIM ' '
#define TITLE_DELIM '-'
#define FILE_ERROR "Error: file not exists or corrupted"
#define INVALID_SCORE "Error: invalid movie score tried to be read from file"
#define MAX 10
#define MIN 1

std::vector<sp_movie> get_titles(const std::string &line)
{
    std::vector<sp_movie> titles;
    std::string movie;
    std::istringstream iss(line);
    while (std::getline(iss, movie, WORDS_DELIM))
    {
        auto separator = movie.find(TITLE_DELIM);
        auto title = movie.substr(0, separator);
        auto year = std::stoi(movie.substr(separator + 1, movie.size()));
        auto spm = std::make_shared<Movie>(title, year);
        titles.push_back(spm);
    }
    return titles;
}

/**
 * a method that gets a string of scores, and coverts it to a vector of double
 * @param scores_string
 * @return
 */
std::vector<double> get_scores(const std::string &scores_string)
{
    std::vector<double> scores;
    std::istringstream scores_stream(scores_string);
    std::string item;
    while (std::getline(scores_stream, item, WORDS_DELIM))
    {
        if (item == "NA" || item == "NA\r" || item == "NA\n")
        {
            scores.push_back(NA);
        } else
        {
            double number;
            std::istringstream iss(item);
            if ((iss >> number).fail())
            {
                throw std::runtime_error(INVALID_SCORE);
            } else if (number < MIN || number > MAX)
            {
                throw std::out_of_range(INVALID_SCORE);
            } else
            {
                scores.push_back(number);
            }
        }

    }
    return scores;
}


RSUser get_user_from_line(std::shared_ptr<RecommenderSystem> &rsp,
                                  std::string &line, rank_map &rankings,
                                  std::vector<sp_movie> &first_line)
{
    auto name = line.substr(0, line.find(' '));
    line.erase(0, line.find(' ') + 1);
    auto titles_scores = get_scores(line);
    auto titles_scores_iter = titles_scores.begin();
    for (const auto &movie: first_line)
    {
        double score = *titles_scores_iter;
        rankings[movie] = score;
        titles_scores_iter++;
    }
    auto user = RSUser(name, rankings, rsp);
    return user;
}

std::vector<RSUser>
RSUsersLoader::create_users_from_file(const std::string &users_file_path,
                                      std::unique_ptr<RecommenderSystem> rs) noexcept(false)
{
    std::ifstream file(users_file_path);
    if (!file)
    {
        throw std::out_of_range(FILE_ERROR);
    } else
    {
        std::string data, line;
        std::vector<RSUser> users;
        std::shared_ptr<RecommenderSystem> rsp = std::move(rs);
        rank_map rankings(1, sp_movie_hash, sp_movie_equal);
        std::stringstream file_data;
        file_data << file.rdbuf();


        if (!std::getline(file_data, line))
        {
            throw std::out_of_range(FILE_ERROR);
        }
        std::vector<sp_movie> first_line = get_titles(line);
        while (std::getline(file_data, line))
        {
            RSUser user = get_user_from_line(rsp, line, rankings, first_line);
            users.push_back(user);
        }
        return users;
    }
}


//
//int main(){
//    RecommenderSystem rs;
//    auto users = RSUsersLoader::create_users_from_file("RSUsersLoader_input.txt", std::make_unique<RecommenderSystem>(rs));
//    std::cout << "hi";
//}
