#pragma once

// [[[0]]] - implement an efficient dynamic array based stack -- template argument must by the type of the data

#define SIZE 10     //default size
#include <stdexcept>

namespace ds
{

    template <typename T>
    struct Stack
    {
        
        private:

            T* data;
            int top,capacity;
            void resize();

        public:

            Stack(int size = SIZE); 
            ~Stack();
            void push(T elem);
            T pop();
            T get_top() const;
            bool is_empty() const;
            bool is_full() const;

    };

    template <typename T>
    void Stack<T> :: resize()       //function to resize the stack when capacity is reached
    {
        capacity *= 2;
        T* new_data = new T[capacity];
        for(int i = 0; i <= top; i++)
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
    }

    template <typename T>
    Stack<T> :: Stack(int size)      //constructor
    {
        capacity = size;
        top = -1;
        data = new T[capacity];
    }

    template <typename T>
    Stack<T> :: ~Stack()        //destructor
    {
        delete[] data;
    }

    template <typename T>
    void Stack<T> :: push(T elem)       //push element on the top of stack,resize if stack is full
    {
        if(is_full())
        resize();       
        data[++top] = elem;
    }

    template <typename T>
    T Stack<T> :: pop()     //pop the element from the top if stack is not empty
    {
        if(is_empty())
            throw std :: out_of_range("Stack is empty!");
        return data[top--];
    }

    template <typename T>
    T Stack<T> :: get_top() const     //getter for top element of stack
    {
        if(is_empty())
            throw std :: out_of_range("Stack is empty!");
        return data[top];
    }   

    template <typename T>
    bool Stack<T> :: is_empty() const    //Checks if stack is empty
    {
        return top == -1;
    }

    template <typename T>
    bool Stack<T> :: is_full() const        //Checks if stack is full
    {
        return top + 1 == capacity;
    }    

}
