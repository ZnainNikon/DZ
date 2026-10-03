#pragma once

#include <stdexcept>
#include <utility>
#include <cstddef>

template <typename T>
class UniquePtr
{
    private:
    T* data;

    public:
    UniquePtr() noexcept : data(nullptr) {}
    UniquePtr(T* data) noexcept : data(data) {}
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other) noexcept : data(other.data) { other.data = nullptr; }

    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this == &other) return *this;
        delete data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }
    T& operator*() const
    {
        if (!data) throw std::runtime_error("Data is void!");
        return *data;
    }
    T* operator->() const
    {
        if (!data) throw std::runtime_error("Data is void!");
        return data;
    }
    T* Get() const noexcept 
    { 
        return data; 
    }
    T* Release() noexcept
    {
        T* ptr = data;
        data = nullptr;
        return ptr;
    }
    void Reset(T* p = nullptr) noexcept
    {
        if (data != p) 
        { 
            delete data; 
            data = p; 
        }
    }
    ~UniquePtr() 
    { 
        delete data; 
    }
};

template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class UniquePtr<T[]>
{
    private:
    T* data;

    public:
    UniquePtr() noexcept : data(nullptr) {}
    UniquePtr(T* data) noexcept : data(data) {}
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other) noexcept : data(other.data) { other.data = nullptr; }

    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    T& operator[](std::size_t i) const 
    { 
        return data[i]; 
    }
    T* Get() const noexcept 
    { 
        return data; 
    }
    T* Release() noexcept
    {
        T* ptr = data;
        data = nullptr;
        return ptr;
    }
    void Reset(T* p = nullptr) noexcept
    {
        if (data != p) 
        { 
            delete[] data; 
            data = p; 
        }
    }
    ~UniquePtr() 
    { 
        delete[] data; 
    }
};

template <typename T>
UniquePtr<T[]> MakeUniqueArray(std::size_t n)
{
    return UniquePtr<T[]>(new T[n]());
}