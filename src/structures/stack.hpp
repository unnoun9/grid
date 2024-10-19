#pragma once
#include<iostream>
#include "int.h"

namespace ds
{

// this stack will probably be used for storing image states and stuff for undo - redo
template <typename T>
struct Stack
{
    Stack(int capacity=2);  // construct a stack of `size`
    ~Stack();               // clear and de-allocate everything

    // push element on the top of stack, resize if stack is full
    void push(const T& elem);

    // pop the element from the top if stack is not empty
    T& pop(bool& success=false);

    // get the top element of the stack
    T& top(bool& success=false) const;

    // getter for capacity and size
    ui32 get_capacity() const;
    ui32 get_size() const;

    // clear the stack; if deallocate is true, then clear the memory otherwise overwrite
    void clear(bool deallocate=false);

    // see whether the stack is empty
    bool is_empty() const;

    // see whether the stack is already full
    bool is_full() const;

private:  
    T* m_data{};            // The actual data
    ui32 m_top = -1;        // pointer/index to the top element
    ui32 m_capacity = 0;    // number of elements that can fit in the current allocated memory
    // to resize the stack when capacity is reached
    void resize();
};

//..................................................................................................
template <typename T>
Stack<T>::Stack(int capacity)
    : m_capacity(capacity)
    , m_data(new T[m_capacity])
{
}

//..................................................................................................
template <typename T>
Stack<T>::~Stack()
{
    delete[] m_data;
    m_data = nullptr;   // for case in which both destructor and clear are called to avoid double deletion
}

//..................................................................................................
template <typename T>
void Stack<T>::push(const T& elem)
{
    if (is_full())
    resize();       
    m_data[++m_top] = elem;
}

//..................................................................................................
template <typename T>
T& Stack<T>::pop(bool& success)     
{
    if (is_empty())
    {
        success = false;
        std::cerr << "Stack is empty! Cannot pop." << std::endl;
        return *reinterpret_cast<T*>(nullptr);  // return null reference
    }
    success = true;
    return m_data[m_top--];
}

//..................................................................................................
template <typename T>
T& Stack<T>::top(bool& success) const     
{
    if(is_empty())
    {
        success = false;
        std::cerr << "Stack is empty! There is no top." << std::endl;
        return *reinterpret_cast<T*>(nullptr);  // return null reference
    }
    success = true;
    return m_data[m_top];
}   

// ..................................................................................................
template <typename T>
ui32 Stack<T>::get_capacity() const 
{
    return m_capacity;
}

// ..................................................................................................
template <typename T>
ui32 Stack<T>::get_size() const 
{
    return m_top+1;
}

// ..................................................................................................
template <typename T>
void Stack<T>::clear(bool deallocate)
{
    if(deallocate)      
    {
        delete[] m_data;
        m_data = nullptr;       // for case in which both destructor and clear are called to avoid double deletion
        m_data = new T[m_capacity];
    }
    m_top = -1;
}

//..................................................................................................
template <typename T>
bool Stack<T>::is_empty() const
{
    return m_top < 0;
}

//..................................................................................................
template <typename T>
bool Stack<T>::is_full() const
{
    return m_top >= m_capacity - 1;
}    

//..................................................................................................
template <typename T>
void Stack<T>::resize()
{
    m_capacity *= 1.5;
    T* new_data = new T[m_capacity];

    for (ui32 i = 0; i <= m_top; i++)
        new_data[i] = m_data[i];

    delete[] m_data;
    m_data = new_data;
}

} // end namespace ds