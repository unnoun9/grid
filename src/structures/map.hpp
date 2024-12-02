#pragma once
#include <iostream>
#include <list>
#include <functional> // for std::hash
#include <utility>    // for std::pair

namespace ds {

template <typename K, typename V>
struct map
{
    // inserts or updates an item
    void insert(const K& key, const V& value);

    // removes an item
    void remove(const K& key);

    // search for an item; if the item is found its value is set to the `value` parameter and true is returned; otherwise nothing is set and false is returned
    bool search(const K& key, V& value) const;

    // check if the hash map is empty
    bool empty() const;

private:
    static const int size = 20; // fixed bucket count
    std::list<std::pair<K, V>> table[size]; // array of buckets (chaining with std::list)

    int hash(const K& key) const
    {
        return std::hash<K>{}(key) % size; // general hash function using std::hash
    }
};

// ..................................................................................................
template <typename K, typename V>
bool map<K, V>::empty() const
{
    for (int i = 0; i < size; i++)
    {
        if (!table[i].empty()) return false; // found a non-empty bucket
    }
    return true;
}

// ..................................................................................................
template <typename K, typename V>
void map<K, V>::insert(const K& key, const V& value)
{
    int hash_value = hash(key);
    auto& bucket = table[hash_value];

    for (auto& entry : bucket)
    {
        if (entry.first == key)
        {
            entry.second = value; // update value
            return;
        }
    }

    bucket.emplace_back(key, value); // add new key-value pair
}

// ..................................................................................................
template <typename K, typename V>
void map<K, V>::remove(const K& key)
{
    int hash_value = hash(key);
    auto& bucket = table[hash_value];

    for (auto itr = bucket.begin(); itr != bucket.end(); ++itr)
    {
        if (itr->first == key)
        {
            bucket.erase(itr); // erase the key-value pair
            return;
        }
    }
}

// ..................................................................................................
template <typename K, typename V>
bool map<K, V>::search(const K& key, V& value) const
{
    int hash_value = hash(key);
    const auto& bucket = table[hash_value];

    for (const auto& entry : bucket)
    {
        if (entry.first == key)
        {
            value = entry.second; // set the value
            return true; // key found
        }
    }

    return false; // key not found
}

} // end namespace ds