//
// Created by PC on 9/10/2022.
//

#ifndef EX6_RON_KOBROWSKI_HASHMAP_HPP
#define EX6_RON_KOBROWSKI_HASHMAP_HPP

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>

#define POWER_OF_2(x) (pow(2, x))
#define START_CAPACITY_RANK 4
#define RECORD_NOT_EXIST "Error: didn't find the record specified"
#define WRONG_VECTOR_DIMS "Error: wrong vector dims"
#define DEFAULT_UPPER 0.75
#define DEFAULT_LOWER 0.25


using namespace std;

template<class KeyT, class ValueT>
class HashMap
{
private:
    double _upper_load_factor{};
    double _lower_load_factor{};
    int _capacity{}; // should always be equal to buckets array size
    typedef int (*hash_function)(KeyT object, int size);

    hash_function func;
    typedef pair<KeyT, ValueT> record;
    typedef list<record> *bucket;
    bucket *buckets; // a pointer to a pointer to list<record>

public:

    /**
     * default ctr
     */
    HashMap()
    {
        _upper_load_factor = DEFAULT_UPPER;
        _lower_load_factor = DEFAULT_LOWER;
        _capacity = POWER_OF_2(START_CAPACITY_RANK);
        // instantiate a dynamic array of size _capacity of list<record>
        buckets = new bucket[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            buckets[i] = new list<record>;
        }
        func = &modulo_hash; // pointer to function
    }

    HashMap(vector<KeyT> keys, vector<ValueT> values): HashMap(){
        if (keys.size() != values.size()){
            throw std::runtime_error(WRONG_VECTOR_DIMS);
        }
        for (unsigned i = 0; i <keys.size(); ++i)
        {
            (*this)[keys[i]] = values[i];
        }
    }

    /**
     * virtual d'tr
     */
    virtual ~HashMap()
    {
        delete_buckets(&buckets, _capacity);
    };

    /**
     *
     * @return the number of elements in the map
     */
    int size() const
    {
        int sum = 0;
        for (int i = 0; i < capacity(); i++)
        {
            sum += buckets[i]->size();
        }
        return sum;
    }


    /**
     * copy ctr
     */
    HashMap(const HashMap &other): HashMap(){
        this->operator=(other);
    }

    /**
     * assigment operator
     * @param other
     * @return
     */
    HashMap& operator=(const HashMap &other) {
        if (&other == this){
            return *this;
        }
        delete_buckets(&buckets, _capacity);
        _capacity = other.capacity();
        _upper_load_factor = other._upper_load_factor;
        _lower_load_factor = other._lower_load_factor;
        func = other.func;
        buckets = other.cpy_buckets();
        return *this;
    }


    bool operator==(const HashMap& other) const{
        for (const auto& t: other){
            if ((*this)[t.first] != t.second){
                return false;
            }
        }
        for (const auto& t: *this){
            if (other[t.first] != t.second){
                return false;
            }
        }
        return true;
    }

    /**
     * opposite of operator==
     * @param other
     * @return
     */
    bool operator!=(const HashMap &other) const{
        return !(*this==other);
    }

    /**
     *
     * @return number of elements the map can hold
     */
    int capacity() const
    {
        return _capacity;
    }

    /**
     *
     * @return true if map not containing any elements
     */
    bool empty() const
    {
        return (size() == 0);
    }

    ValueT& at(KeyT key) const
    {
        auto rec = find_key(key);
        if(!rec){throw out_of_range(WRONG_VECTOR_DIMS);}
        return rec->second;
    }

    /**
     * activates find_key
     * @param key
     * @return true if contains key
     */
    bool contains_key(KeyT key) const
    {
        return find_key(key);
    }

    double get_load_factor() const
    {
        return (double) size() / (double) capacity();
    }

    /**
     * inserts key, value to the map
     * @param key
     * @param value
     * @return true if values wasn't on the key, and was added successfully
     */
    bool insert(KeyT key, ValueT value)
    {
        if (!contains_key(key))
        {
            bucket b = (buckets[bucket_index(key)]);
            b->push_back(std::make_pair(key, value));
            if (get_load_factor() > _upper_load_factor)
            { increase_capacity(); }
            return true;
        } else
        {
            return false;
        }

    }

    /**
     * return the value at key, or create a new value at this key
     * @param key
     * @return
     */
    ValueT &operator[](KeyT key)
    {
        // kinda dangerous for infinite loops
        auto item = find_key(key); // pointer to the right record
        if (!item)
        { // then inserts an empty record and recall the function
            if (!insert(key, ValueT()))
            {
                throw runtime_error(RECORD_NOT_EXIST);
            } // inserts a (key,empty value) record
            return operator[](key); // re-running should return the value
        } else
        {
            return (item->second);
        } // reference to the value of the record
    }

    ValueT operator[](KeyT key) const
    {
        // kinda dangerous for infinite loops
        auto item = find_key(key); // pointer to the right record
        if (!item)
        { // then inserts an empty record and recall the function
            return ValueT();
            return operator[](key); // re-running should return the value
        } else
        {
            return (item->second);
        } // reference to the value of the record
    }

    /**
     * using the hash function
     * @param key
     * @return
     */
    int bucket_index(KeyT key) const
    {
        auto a = func(key, capacity());
        return a;
    }

    int bucket_size(KeyT key)
    {
        return buckets[bucket_index(key)]->size();
    }

    /**
     * removes a key,value from database
     * @param key
     * @return
     */
    virtual bool erase(KeyT key)
    {
        auto &b = *(buckets[bucket_index(key)]);
        for (auto position = b.begin(); position != b.end(); position++)
        {
            if (position->first == key)
            {
                auto temp = position;
                position++;
                b.erase(temp);
                while (get_load_factor() < _lower_load_factor && _capacity > 1)
                { decrease_capacity(); }
                return true;
            }
        }
        return false;
    }

    /**
     * deletes buckets, and create empty new ones
     */
    void clear()
    {
        delete_buckets(&buckets, _capacity);
        buckets = new bucket[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            buckets[i] = new list<record>;
        }
    }




    class ConstIterator
    {
    private:
        typename list<record>::const_iterator current_record;
        int current_bucket{};
        const HashMap<KeyT, ValueT> &current_hash_map;
    public:
        typedef pair<KeyT, ValueT> value_type;
        typedef const pair<KeyT, ValueT> &reference;
        typedef const pair<KeyT, ValueT> *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        /**
         * aux method for advancing iterator to the next element in container
         */
        void find_next()
        {
            while ((current_hash_map.buckets[current_bucket]->size() == 0 &&
                    current_bucket < current_hash_map.capacity() - 1) ||
                   current_record ==
                   current_hash_map.buckets[current_bucket]->end())
            {
                current_bucket++;
                if (current_bucket >= current_hash_map.capacity() - 1)
                {
                    current_bucket = -1;
                    current_record = current_hash_map.buckets[
                            current_hash_map.capacity() - 1]->end();
                    return;
                }
                current_record = current_hash_map.buckets[current_bucket]
                        ->begin();
                if (current_hash_map.buckets[current_bucket]->size() != 0)
                {
                    return; // start of a new bucket
                }
            }


            // if here, never entered the first while
            current_record++;
            if (current_record ==
                current_hash_map.buckets[current_bucket]->end())
            {
                find_next();
            }
        }

        ConstIterator(typename list<record>::const_iterator rc, int bucket,
                      const HashMap<KeyT, ValueT> &hm) :
                current_record(rc), current_bucket(bucket),
                current_hash_map(hm)
        {
            find_next();
        }

        explicit ConstIterator(const HashMap<KeyT, ValueT> &hm) :
                ConstIterator((hm.buckets)[0]->begin(), 0, hm)
        {}


        ConstIterator &operator++()
        {
            find_next();
            return *this;
        }

        ConstIterator operator++(int unused)
        {
            auto tmp_record = current_record;
            auto tmp_bucket = current_bucket;
            auto tmp = ConstIterator(tmp_record, tmp_bucket, current_hash_map);
            find_next();
            return tmp;
        }

        bool operator==(const ConstIterator &rhs) const
        {
            return current_record == rhs.current_record;
        }

        bool operator!=(const ConstIterator &rhs) const
        {
            return current_record != rhs.current_record;
        }

        reference operator*() const
        { return *current_record; }

        pointer operator->() const
        { return &(operator*()); }

    };

    using const_iterator = ConstIterator;

    const_iterator begin() const
    { return ConstIterator(*this); }

    const_iterator end() const
    {
        return ConstIterator(buckets[_capacity - 1]->end(), _capacity - 1,
                             *this);
    }const_iterator cbegin() const
    { return ConstIterator(*this); }

    const_iterator cend() const
    {
        return ConstIterator(buckets[_capacity - 1]->end(), _capacity - 1,
                             *this);
    }

private:

    /**
     * given hash function
     * @param key
     * @param size
     * @return
     */
    static int modulo_hash(KeyT key, int size)
    {
        auto v = std::hash<KeyT>{}(key);
        return v & (size - 1);
    }

    /**
     * fills objects database with old_buckets
     * @param old_buckets
     * @param old_size
     */
    void rehash(const bucket *old_buckets, int old_size)
    {
        for (int i = 0; i < old_size; i++)
        {
            for (auto j: (*old_buckets[i]))
            {
                insert(j.first, j.second);
            }
        }
    }

    /**
     * return a pointer to the record of the given key
     * @param key
     * @return
     */
    record *find_key(KeyT key) const
    {
        auto &b = *(buckets[bucket_index(key)]);
        for (auto it = b.begin(); it != b.end(); it++)
        {
            if (it->first == key)
            {
                return &(*it); // should be a pointer to the record
            }
        }
        return nullptr;
    }

    /**
     * increase capacity and rehash once
     */
    void increase_capacity()
    {
        auto old_buckets = cpy_buckets();
        auto old_size = capacity();
        delete_buckets(&buckets, old_size);
        _capacity = _capacity * 2;
        buckets = new bucket[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            buckets[i] = new list<record>;
        }
        rehash(old_buckets, old_size);
        delete_buckets(&old_buckets, old_size);
    }

    /**
     * function that decreases capacicty and rehashes once
     */
    void decrease_capacity()
    {
        if (_capacity < 2)
        {
            return;
        }
        auto old_buckets = cpy_buckets();
        auto old_size = capacity();
        delete_buckets(&buckets, old_size);
        _capacity = _capacity / 2;
        buckets = new bucket[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            buckets[i] = new list<record>;
        }
        rehash(old_buckets, old_size);
        delete_buckets(&old_buckets, old_size);
    }

    /**
     * aux method for creating a copy of buckets
     * @return
     */
    bucket *cpy_buckets() const
    {
        auto ret = new bucket[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            ret[i] = new list<record>;
            *(ret[i]) = *(buckets[i]);
        }
        return ret;
    }

    /**
     * aux method for deleting allocated buckets, could be static
     * @param p_buckets
     * @param size
     */
    void delete_buckets(bucket **p_buckets, int size)
    {
        for (int i = 0; i < size; i++)
        {
            delete (*p_buckets)[i];
        }
        delete[] *p_buckets;
    }

};


#endif //EX6_RON_KOBROWSKI_HASHMAP_HPP
