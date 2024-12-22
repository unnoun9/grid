#pragma once
#include <iostream>
#include "int.h"

namespace ds
{

template <typename T>
struct Node
{
    T data;
    Node* prev = nullptr;
    Node* next = nullptr;
};

template <typename T>
class DoublyLinkedList
{
public:
    // Constructor
    DoublyLinkedList();
    
    // Destructor
    ~DoublyLinkedList();

    // Insert element at the front of the list
    void insert_front(const T& elem);
    
    // Insert element at the back of the list
    void insert_back(const T& elem);

    // Remove and return element from the front of the list
    T remove_front(bool& success);
    
    // Remove and return element from the back of the list
    T remove_back(bool& success);

    // Get the front element of the list
    T get_front(bool& success) const;
    
    // Get the back element of the list
    T get_back(bool& success) const;

    // Check if the list is empty
    bool is_empty() const;
    
    // Get the current size of the list
    ui32 get_size() const;
    
    // Clear the list and free memory
    void clear();
    
private:
    Node<T>* m_front = nullptr;
    Node<T>* m_back = nullptr;
    ui32 m_size = 0;
};

// ..................................................................................................
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : m_front(nullptr), m_back(nullptr), m_size(0) {}

// ..................................................................................................
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    clear();
}

// ..................................................................................................
template <typename T>
void DoublyLinkedList<T>::insert_front(const T& elem)
{
    Node<T>* new_node = new Node<T>{elem};
    if (is_empty())
    {
        m_front = m_back = new_node;
    }
    else
    {
        new_node->next = m_front;
        m_front->prev = new_node;
        m_front = new_node;
    }
    m_size++;
}

// ..................................................................................................
template <typename T>
void DoublyLinkedList<T>::insert_back(const T& elem)
{
    Node<T>* new_node = new Node<T>{elem};
    if (is_empty())
    {
        m_front = m_back = new_node;
    }
    else
    {
        new_node->prev = m_back;
        m_back->next = new_node;
        m_back = new_node;
    }
    m_size++;
}

// ..................................................................................................
template <typename T>
T DoublyLinkedList<T>::remove_front(bool& success)
{
    if (is_empty())
    {
        success = false;
        return T(); // Return default-constructed value
    }
    success = true;
    Node<T>* temp = m_front;
    T data = temp->data;
    m_front = m_front->next;
    if (m_front)
        m_front->prev = nullptr;
    else
        m_back = nullptr; // List is now empty
    delete temp;
    m_size--;
    return data;
}

// ..................................................................................................
template <typename T>
T DoublyLinkedList<T>::remove_back(bool& success)
{
    if (is_empty())
    {
        success = false;
        return T(); // Return default-constructed value
    }
    success = true;
    Node<T>* temp = m_back;
    T data = temp->data;
    m_back = m_back->prev;
    if (m_back)
        m_back->next = nullptr;
    else
        m_front = nullptr; // List is now empty
    delete temp;
    m_size--;
    return data;
}

// ..................................................................................................
template <typename T>
T DoublyLinkedList<T>::get_front(bool& success) const
{
    if (is_empty())
    {
        success = false;
        return T(); // Return default-constructed value
    }
    success = true;
    return m_front->data;
}

// ..................................................................................................
template <typename T>
T DoublyLinkedList<T>::get_back(bool& success) const
{
    if (is_empty())
    {
        success = false;
        return T(); // Return default-constructed value
    }
    success = true;
    return m_back->data;
}

// ..................................................................................................
template <typename T>
bool DoublyLinkedList<T>::is_empty() const
{
    return m_size == 0;
}

// ..................................................................................................
template <typename T>
ui32 DoublyLinkedList<T>::get_size() const
{
    return m_size;
}

// ..................................................................................................
template <typename T>
void DoublyLinkedList<T>::clear()
{
    while (!is_empty())
    {
        bool success;
        remove_front(success);
    }
}

} // end namespace ds
