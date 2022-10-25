//
// Created by PC on 9/10/2022.
//
#include "HashMap.hpp"

int main()
{
    auto h = HashMap<int, int>();
    for (int i = 0 ; i<10; i++){
        h.insert(i+1, i+1*2);
    }
    auto b(h);
//    b[9] = 15;
    auto l = (b==h);
    cout<< "are equal: " << l << endl;
//    h.insert("ron", "kobrowski");
//    h.insert("shir", "kobrowski");

    for (const auto & t : b){
        cout << t.first << " " << t.second << endl;
    }
    cout << endl << h.size() << " " << h.capacity() << endl;

}