//
// Created by PC on 9/7/2022.
//
#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"
int main(){
    auto rsp = RecommenderSystemLoader::create_rs_from_movies_file("RecommenderSystemLoader_input.txt");
//    std::shared_ptr<RecommenderSystem> sharedPtr = std::move(rsp);
    auto users = RSUsersLoader::create_users_from_file("RSUsersLoader_input.txt", std::move(rsp));
    auto mov =users[2].get_prediction_score_for_movie ("Twilight", 2008, 2);
    std::cout<<mov;
}