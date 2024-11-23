#include<iostream>
#include "../src/structures/map.hpp"

int main() {
    hash_map<int, std::string> map;

    // Insert items
    map.insertItem(1, "Alice");
    map.insertItem(2, "Bob");
    map.insertItem(3, "Charlie");

    // Print the hash map
    std::cout << "Hash Map Contents:\n";
    map.printTable();

    // Search for items
    try {
        std::cout << "\nValue for key 2: " << map.searchTable(2) << "\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }

    // Remove an item
    map.removeItem(2);

    // Print after removal
    std::cout << "\nHash Map Contents after removal:\n";
    map.printTable();

    // Check if map is empty
    std::cout << "\nIs map empty? " << (map.isEmpty() ? "Yes" : "No") << "\n";

    // Try to access a removed key
    try {
        std::cout << "Value for key 2: " << map.searchTable(2) << "\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
