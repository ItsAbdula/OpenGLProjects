#pragma once

// Not Thread-safe
template <class T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton &) = delete;
    //Singleton(Singleton &&) = delete;
    //Singleton & operator=(Singleton &&) = delete;

    static T& getInstance()
    {
        static T *instance = new T();

        return *instance;
    }
protected:
    Singleton() {}
};