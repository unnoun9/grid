#pragma once
#include<iostream>

#include "int.h"

// [[[0]]] - implement an efficient dynamic array based stack -- template argument must by the type of the data

#define SIZE 10 //default size
#define SIZE_Pi 50000000    //default size of pixel array

namespace ds
{

// this stack will probably be used for storing image states and stuff for undo - redo
template <typename T>
struct Stack
{
    Stack(int size = SIZE); 
    ~Stack();

    // push element on the top of stack, resize if stack is full
    void push(T* elem);

    // pop the element from the top if stack is not empty
    T* pop();

    // get the top element of the stack
    T* get_top() const;

    // display the top element of the stack e.g pixel array
    void display_top();

    // check whether or not the stack is empty or full
    bool is_empty() const;
    bool is_full() const;

private:
    // The actual data
    T* data;
    // pointer/index to the top element
    i32 top;
    // number of elements that can fit in the current allocated memory
    i32 capacity;
    
    // function to resize the stack when capacity is reached
    void resize();
};

// ..................................................................................................
template <typename T>
void Stack<T>::resize()
{
    capacity *= 1.5;
    T* new_data = new T[capacity];
    for(int i = 0; i <= top; i++)
        new_data[i] = data[i];
    delete[] data;
    data = new_data;
}

// ..................................................................................................
template <typename T>
Stack<T>::Stack(int size)
{
    capacity = size;
    top = -1;
    data = new T[capacity];
}

// ..................................................................................................
template <typename T>
Stack<T>::~Stack()
{
    delete[] data;
}

// ..................................................................................................
template <typename T>
void Stack<T>::push(T* elem)
{
    if(is_full())
    resize();       
    data[++top] = *elem;
}

// ..................................................................................................
template <typename T>
T* Stack<T>::pop()     
{
    if(is_empty())
    {
        std::cerr << "Stack is empty!" << std :: endl;
        return nullptr;
    }
    return &data[top--];
}

// ..................................................................................................
template <typename T>
T* Stack<T>::get_top() const     
{
    if(is_empty())
    {
        std::cerr << "Stack is empty!" << std :: endl;
        return nullptr;
    }
    return &data[top];
}   

// ..................................................................................................
template <typename T>
void Stack<T>::display_top()
{
    if(is_empty())
    {
        std::cerr << "Stack is empty!" << std :: endl;
        return;
    }
    T top_element = data[top];
    for(int i = 0; i < SIZE_Pi; i++)
        std :: cout << top_element[i] << " ";
    std :: cout << std :: endl;
}

// ..................................................................................................
template <typename T>
bool Stack<T>::is_empty() const
{
    return top == -1;
}

// ..................................................................................................
template <typename T>
bool Stack<T>::is_full() const
{
    return top + 1 == capacity;
}    

}