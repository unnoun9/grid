#pragma once
#include <iostream>
#include <list>
#include <functional> // For std::hash
#include <utility>    // For std::pair

template <typename K, typename V>
class hash_map {
private:
    static const int size = 20; // Fixed bucket count
    std::list<std::pair<K, V>> table[size]; // Array of buckets (chaining with std::list)

    int hashFunction(const K& key) const {
        return std::hash<K>{}(key) % size; // General hash function using std::hash
    }

public:
    bool isEmpty() const;
    void insertItem(const K& key, const V& value);
    void removeItem(const K& key);
    V searchTable(const K& key) const;
    void printTable() const;
};

// Check if the hash map is empty
template <typename K, typename V>
bool hash_map<K, V>::isEmpty() const {
    for (int i = 0; i < size; i++) {
        if (!table[i].empty()) return false; // Found a non-empty bucket
    }
    return true;
}

// Insert or update an item
template <typename K, typename V>
void hash_map<K, V>::insertItem(const K& key, const V& value) {
    int hashValue = hashFunction(key);
    auto& bucket = table[hashValue];

    for (auto& entry : bucket) {
        if (entry.first == key) {
            entry.second = value; // Update value
            std::cout << "Key exists. Value is updated.\n";
            return;
        }
    }

    bucket.emplace_back(key, value); // Add new key-value pair
    std::cout << "Key inserted.\n";
}

// Remove an item
template <typename K, typename V>
void hash_map<K, V>::removeItem(const K& key) {
    int hashValue = hashFunction(key);
    auto& bucket = table[hashValue];

    for (auto itr = bucket.begin(); itr != bucket.end(); ++itr) {
        if (itr->first == key) {
            bucket.erase(itr); // Erase the key-value pair
            std::cout << "Item removed.\n";
            return;
        }
    }

    std::cout << "Key not found.\n";
}

// Search for an item
template <typename K, typename V>
V hash_map<K, V>::searchTable(const K& key) const {
    int hashValue = hashFunction(key);
    const auto& bucket = table[hashValue];

    for (const auto& entry : bucket) {
        if (entry.first == key) {
            return entry.second; // Return the value
        }
    }

    throw std::runtime_error("Key not found."); // Key does not exist
}

// Print the hash map
template <typename K, typename V>
void hash_map<K, V>::printTable() const {
    for (int i = 0; i < size; i++) {
        if (table[i].empty()) continue;

        std::cout << "Bucket " << i << ": ";
        for (const auto& entry : table[i]) {
            std::cout << "[Key: " << entry.first << ", Value: " << entry.second << "] ";
        }
        std::cout << "\n";
    }
}

