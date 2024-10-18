#pragma once
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
    void push(T* elem);

    // pop the element from the top if stack is not empty
    T* pop();

    // get the top element of the stack
    T* top() const;

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

//..................................................................................................
template <typename T>
void Stack<T>::push(T* elem)
{
    if (is_full())
    resize();       
    m_data[++m_top] = *elem;
}

//..................................................................................................
template <typename T>
T* Stack<T>::pop()     
{
    if (is_empty())
    {
        std::cerr << "Stack is empty!" << std :: endl;
        return nullptr;
    }
    return &m_data[m_top--];
}

//..................................................................................................
template <typename T>
T* Stack<T>::top() const     
{
    if (is_empty())
    {
        std::cerr << "Stack is empty!" << std :: endl;
        return nullptr;
    }
    return &m_data[m_top];
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

} // end namespace ds