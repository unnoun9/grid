#pragma once
#include<iostream>
#include "int.h"

#define SIZE 10 //default size

namespace ds
{

// circular queue for storing image states
template <typename T>
struct Queue
{
    Queue(int capacity = SIZE); 
    ~Queue();

    // push element at the rear of the queue, resize if queue is full
    void enqueue(T& elem);

    // pop element from the front
    T& dequeue(bool& success=false);

    // get front element
    T& get_front(bool& success=false) const;

    // getters for current_size of queue and current_capacity
    ui32 get_size() const;
    ui32 get_capacity() const;

    // clear the queue
    void clear(bool deallocate=false);

    // check whether or not the queue is empty or full
    bool is_empty() const;
    bool is_full() const;

private:
    // the actual data
    T* m_data{};
    // index of the front element 
    ui32 m_front=0;
    // index of the rear element 
    ui32 m_rear=0;
    // maximum number of elements that can fit in the current allocated memory
    ui32 m_capacity=0;
    // current number of elements 
    ui32 m_count=0; 

    // function to resize the queue when capacity is reached
    void resize();
};

// ..................................................................................................
template <typename T>
Queue<T>::Queue(int capacity)
    : m_capacity(capacity)
    , m_data(new T[m_capacity])
{
}

// ..................................................................................................
template <typename T>
Queue<T>::~Queue()
{
    delete[] m_data;
    m_data = nullptr;   // for case in which both destructor and clear are called to avoid double deletion
}

// ..................................................................................................
template <typename T>
void Queue<T>::enqueue(T& elem)
{
    if (is_full())
        resize();
    
    m_data[m_rear] = elem;
    m_rear = (m_rear + 1) % m_capacity;
    m_count++;
}

// ..................................................................................................
template <typename T>
T& Queue<T>::dequeue(bool& success)
{
    if (is_empty())
    {
        success = false;
        std::cerr << "Queue is empty! Cannot dequeue." << std::endl;
        return *reinterpret_cast<T*>(nullptr);  // return null reference
    }
    success = true;
    T& elem = m_data[m_front];
    m_front = (m_front + 1) % m_capacity;
    m_count--;
    return elem;
}

// ..................................................................................................
template <typename T>
T& Queue<T>::get_front(bool& success) const
{
    if(is_empty())
    {
        success = false;
        std::cerr << "Queue is empty! No front element present." << std::endl;
        return *reinterpret_cast<T*>(nullptr);  // return null reference
    }
    success = true;
    return m_data[m_front];   
}

// ..................................................................................................
template <typename T>
ui32 Queue<T>::get_size() const 
{
    return m_count;
}

// ..................................................................................................
template <typename T>
ui32 Queue<T>::get_capacity() const 
{
    return m_capacity;
}

// ..................................................................................................
template <typename T>
void Queue<T>::clear(bool deallocate)
{
    if(deallocate)
    {
        delete[] m_data;
        m_data = nullptr;   // for case in which both destructor and clear are called to avoid double deletion
        m_data = new T[m_capacity];
    }
    m_front = 0;
    m_rear = 0;
    m_count = 0;
}

// ..................................................................................................
template <typename T>
bool Queue<T>::is_empty() const
{
    return m_count == 0;
}

// ..................................................................................................
template <typename T>
bool Queue<T>::is_full() const
{
    return m_count == m_capacity;
}

// ..................................................................................................
template <typename T>
void Queue<T>::resize()
{
    m_capacity *= 1.5;
    T* new_data = new T[m_capacity];
    for (ui32 i = 0; i < m_count; i++)
        new_data[i] = m_data[(m_front + i) % m_capacity];
    delete[] m_data;
    m_data = new_data;
    m_front = 0;
    m_rear = m_count;
}

} // end namespace ds
