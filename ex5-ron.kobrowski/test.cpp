#include "Movie.h"
#include "assert.h"
#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"
#include "string"

void check_geters ()
{
  Movie m1 ("Twilight", 2008);
  Movie m2 ("Titanic", 1997);
  Movie m3 ("Batman", 2022);
  int x = m3.get_name ().std::string::compare ("Batman");
  assert(m3.get_year () == 2022);
  assert(x == 0);
  assert(m1.get_year () == 2008);
};

void check_op ()
{
  Movie m1 ("Twilight", 1997);
  Movie m2 ("Titanic", 1997);
  Movie m3 ("Batman", 2022);
  Movie m4 ("Batman", 1099);

  assert((m4 < m3));
  assert(!(m3 < m4));
  assert(m2 < m1);
}

void check_getters_rs ()
{
  Movie m1 ("Twilight", 2008);
  Movie m2 ("Titanic", 1997);
  Movie m3 ("Batman", 2022);
  auto p = RecommenderSystemLoader::create_rs_from_movies_file("RecommenderSystemLoader_input.txt");
  auto temp1 = p->get_movie ("Titanic", 1997);
  assert(temp1->get_year () == 1997);
  auto temp2 = p->get_movie ("Asd", 2005);
  assert(!temp2);
  auto temp3 = p->get_movie ("Batman", 2024);
  assert(!temp3);

}

void check_add_rs ()
{
  std::vector<double> v1 = {4, 5, 6, 8};
  Movie m1 ("Twilight", 2008);
  Movie m2 ("Titanic", 1997);
  Movie m3 ("Batman", 2022);
  auto p = RecommenderSystemLoader::create_rs_from_movies_file
      ("RecommenderSystemLoader_input.txt");
  p->add_movie ("Dorf", 2021, v1);
  auto temp1 = p->get_movie ("Dorf", 2021);
  assert(temp1->get_year () == 2021);
}

void check_users_names ()
{
  std::string rs_point = "RecommenderSystemLoader_input.txt";
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file("RSUsersLoader_input.txt", RecommenderSystemLoader::create_rs_from_movies_file
      (rs_point));
  assert(users[0].get_name () == "Sofia");
  assert(users[1].get_name () == "Michael");
  assert(users[2].get_name () == "Nicole");
  assert(users[3].get_name () == "Arik");
  assert(users.size () == 4);

};

void check_users_func ()
{
  std::string rs_point = "RecommenderSystemLoader_input.txt";
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file
      ("RSUsersLoader_input.txt", RecommenderSystemLoader::create_rs_from_movies_file
      (rs_point));
  assert(users[2].get_recommendation_by_cf (2)->get_name () == "Titanic");
  assert(users[0].get_recommendation_by_content ()->get_name () == "Batman");

  assert(5.4 < users[2].get_prediction_score_for_movie ("Titanic", 1997, 2)
         && users[2].get_prediction_score_for_movie ("Titanic", 1997, 2)
            < 5.5);
  assert(users[2].get_prediction_score_for_movie ("Titanic", 1997, 1) == 5);
  assert(3.4 < users[2].get_prediction_score_for_movie ("Twilight", 2008, 2) &&
         users[2].get_prediction_score_for_movie ("Twilight", 2008, 2) < 3.55);

}

void empy_test_func()
{
  std::vector<double> d = {2, 5, 7, 8};
  std::vector<double> d1 = {3 ,4 ,5 ,6};
  std::string rs_point = "empty_test";
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file
      ("empty_user_test", RecommenderSystemLoader::create_rs_from_movies_file
      (rs_point));

  users[0].add_movie_to_rs ("Goal", 2002, d, 8);
  assert(users[0].get_recommendation_by_content()->get_name() == "Bono");
  assert(users[1].get_recommendation_by_content()->get_name() == "Superman");
  users[1].add_movie_to_rs ("Boom", 2002, d1, 4);
  assert(users[1].get_recommendation_by_content() != nullptr);
  users[1].add_movie_to_rs ("Goal", 2002, d, 4);
}

void exception_test()
{
  std::vector<double> d = {2, 5, 7, 8};
  int check = 0;
  try
    {
      std::vector<RSUser> users = RSUsersLoader::create_users_from_file
          ("empty_user_test", RecommenderSystemLoader::create_rs_from_movies_file
              ("not good"));
    }
    catch (std::runtime_error&)
      {
        check = 1;
      }
  assert(check == 1);

  std::string rs_point = "empty_test";
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file
      ("empty_user_test", RecommenderSystemLoader::create_rs_from_movies_file
          (rs_point));

  int valid = 0;
  try
    {
      users[0].add_movie_to_rs ("Goal", 2002, d, 11);
    }
  catch (std::runtime_error&)
    {
      valid = 1;
    }
  assert(valid == 1);

  int last_exception = 0;
  try
    {
      users[0].add_movie_to_rs ("Goal", 2002, d, 0);
    }
  catch (std::runtime_error&)
    {
      last_exception = 1;
    }
  assert(last_exception == 1);
};

void other_test()
{
  std::string rs_point = "test_movies_input";
  std::vector<RSUser> users = RSUsersLoader::create_users_from_file
      ("test_users_input", RecommenderSystemLoader::create_rs_from_movies_file
          (rs_point));
  assert(6 < users[0].get_prediction_score_for_movie ("Bono", 2008, 2) &&
             users[0].get_prediction_score_for_movie ("Bono", 2008, 2) < 6.1);
  assert(users[0].get_recommendation_by_cf (2)->get_name() == "StarWars") ;
  assert(users[3].get_recommendation_by_content()->get_name() == "StarWars");
  assert(users[2].get_recommendation_by_content()->get_name() == "Bono");
  assert(users[1].get_recommendation_by_content()->get_name() == "Inception");
  assert(6.9 < users[1].get_prediction_score_for_movie ("Inception", 2000, 3) &&
         users[0].get_prediction_score_for_movie ("Inception", 2000, 3) < 7);
  int valid = 0;
  try
    {
      auto temp = users[1].get_prediction_score_for_movie ("Incepxtion", 2000, 3);
    }
  catch(std::runtime_error&) {
    valid = 1;
      assert(valid == 1);
  }
  int tried = 0;
  try
    {
      auto temp = users[1].get_prediction_score_for_movie ("Inception", 2010, 3);
    }
  catch(std::runtime_error&) {
      tried = 1;
      assert(tried == 1);
    }
};

int main ()
{
  check_geters ();

  check_op ();

  check_getters_rs ();

  check_add_rs ();

  check_users_names ();

  check_users_func ();

  empy_test_func();

  exception_test();

  other_test();

}

