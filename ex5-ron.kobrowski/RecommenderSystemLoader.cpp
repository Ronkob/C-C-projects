//
// Created by PC on 9/6/2022.
//
#include "RecommenderSystemLoader.h"
#include <fstream>
#include <sstream>

#define WORDS_DELIM ' '
#define TITLE_DELIM '-'
#define FILE_ERROR "Error: file not exists or corrupted"
#define INVALID_SCORE "Error: invalid movie score tried to be read from file"
#define MAX 10
#define MIN 1


std::vector<double> get_features(const std::string& line){
    std::vector<double> scores;
    std::istringstream scores_stream(line);
    std::string item;
    while (std::getline(scores_stream, item, WORDS_DELIM))
    {
        double number;
        std::istringstream iss(item);
        if ((iss >> number).fail())
        {
            throw std::runtime_error(INVALID_SCORE);
        } else if (number < MIN || number > MAX)
        {
            throw std::runtime_error(INVALID_SCORE);
        } else
        {
            scores.push_back(number);
        }
    }
    return scores;
}

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file(
        const std::string &movies_file_path) noexcept(false)
{
    std::ifstream file(movies_file_path);
    if (!file)
    {
        throw std::runtime_error(FILE_ERROR);
    } else
    {
        RecommenderSystem rs;
        std::string line;
        while(std::getline(file, line)){
            std::string movie = line.substr(0, line.find(WORDS_DELIM));
            auto separator = movie.find(TITLE_DELIM);
            auto title = movie.substr(0, separator);
            auto year = std::stoi(movie.substr(separator + 1, movie.size()));
            auto spm = std::make_shared<Movie>(title, year);
            auto features = get_features(line.substr(line.find(WORDS_DELIM)+1, line.size()));
            rs.add_movie(title, year, features);
        }
        return std::make_unique<RecommenderSystem>(rs);
    }
}
//
//int main(){
//    auto rsp = RecommenderSystemLoader::create_rs_from_movies_file("RecommenderSystemLoader_input.txt");
//    std::cout<<4;
//}

