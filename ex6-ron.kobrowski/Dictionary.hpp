//
// Created by PC on 9/11/2022.
//

#ifndef EX6_RON_KOBROWSKI_DICTIONARY_HPP
#define EX6_RON_KOBROWSKI_DICTIONARY_HPP

#include "HashMap.hpp"
#define INVALID_KEY "Error: invalid key"

/**
 * child class of invalid_argument
 */
class InvalidKey : public invalid_argument{ ;
public:
    explicit InvalidKey() : invalid_argument(INVALID_KEY), msg_(INVALID_KEY){}
    explicit InvalidKey(const string &arg)
            : invalid_argument(arg)
    {
    }
protected:
    string msg_;
};


class Dictionary : public HashMap<string, string>{
public:
    // for using the c'trs
    using HashMap<string, string>::HashMap;

    // overloading virtual method
    bool erase(string key) override{
        bool b = HashMap::erase(key);
        if(!b) {throw InvalidKey(INVALID_KEY);}
        return true;
    }

    template<typename Iter>
    void update(Iter start, Iter end){
        for (; start!=end; start++){
            (*this)[start->first] = start->second;
        }
    }
};


#endif //EX6_RON_KOBROWSKI_DICTIONARY_HPP
