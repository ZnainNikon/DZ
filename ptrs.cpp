#include <iostream>
#include <stdexcept>
#include <memory>
#include <type_traits>
 
template <typename T>
class UniquePtr
{
    private:
    T* data;

    public:
    UniquePtr() : data(nullptr) {}
    UniquePtr(T* data) : data(data) {}
    UniquePtr(const UniquePtr& other) = delete; 
    UniquePtr(UniquePtr&& other) : data(std::move(other.data)) {other.data = nullptr;}
    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (this == &other) return *this; 
        this->data = other.data;
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
    void Reset() 
    {
        delete data;
    }
    ~UniquePtr() 
    {
        delete data;
    }
};

template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args)  
{
    return new T(std::forward<Args>(args)...);
}
 
template <typename T>
class SharedPtr
{
    private:
    T* data;
    std::size_t* referenceCount;

    public:
    SharedPtr() : data(nullptr), referenceCount(nullptr) {}
    SharedPtr(T* data) : data(data), referenceCount(new std::size_t(1)) {}
    SharedPtr(const SharedPtr& other) : data(other.data), referenceCount(other.referenceCount)
    {
        if (this->referenceCount) (*this->referenceCount)++;
    }
    SharedPtr(SharedPtr&& other) : data(other.data), referenceCount(other.referenceCount)
    {
        other.data = nullptr;
        other.referenceCount = nullptr;
    }
    SharedPtr& operator=(const SharedPtr& other)
    {
        this->data = other.data;
        this->referenceCount = other.referenceCount;
        if (referenceCount) (*referenceCount)++;
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other)
    {
        this->data = other.data;
        this->referenceCount = other.referenceCount;
        other.data = nullptr; 
        other.referenceCount = nullptr;
        return *this;
    }
    T& operator*() const 
    {
        if (!this->data) throw std::runtime_error("Data is void!");
        return *data;
    }
    T* operator->() const noexcept
    {
        return this->data;
    }
    T* Get() const noexcept
    {
        return this->data;
    }
    std::size_t UseCount() const 
    {
        return *this->referenceCount;
    }
    ~SharedPtr() 
    {
        (*this->referenceCount)--;
        if (this->data && this->referenceCount && (*this->referenceCount) == 0) 
        {
            delete this->data; 
            delete this->referenceCount; 
        }
    }
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
    return new T(std::forward<Args>(args)...);
}

template <typename T>
class UniquePtrArray
{
    private:
    T* data;

    public:
    UniquePtrArray() : data(nullptr) {}
    UniquePtrArray(T* data) : data(data) {}
    UniquePtrArray(const UniquePtrArray& other) = delete;
    UniquePtrArray(UniquePtrArray&& other) noexcept : data(other.data) { other.data = nullptr; }

    UniquePtrArray& operator=(const UniquePtrArray& other) = delete;
    UniquePtrArray& operator=(UniquePtrArray&& other) noexcept
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
    ~UniquePtrArray() 
    { 
        delete[] data; 
    }
};

template <typename T>
UniquePtrArray<T> MakeUniqueArray(std::size_t n)
{
    return UniquePtrArray<T>(new T[n]());
}

template <typename T>
class SharedPtrArray//class SharedPtr<T[]>
{
    private:
    T* data;
    std::size_t* referenceCount;

    void ReleaseRef() noexcept
    {
        if (referenceCount && --(*referenceCount) == 0)
        {
            delete[] data;
            delete referenceCount;
        }
        data = nullptr;
        referenceCount = nullptr;
    }

    public:
    SharedPtrArray() : data(nullptr), referenceCount(nullptr) {}
    SharedPtrArray(T* data) : data(data), referenceCount(new std::size_t(1)) {}
    SharedPtrArray(const SharedPtrArray& other) : data(other.data), referenceCount(other.referenceCount)
    {
        if (referenceCount) (*referenceCount)++;
    }
    SharedPtrArray(SharedPtrArray&& other) noexcept : data(other.data), referenceCount(other.referenceCount)
    {
        other.data = nullptr;
        other.referenceCount = nullptr;
    }
    SharedPtrArray& operator=(const SharedPtrArray& other)
    {
        if (this == &other) return *this;
        ReleaseRef();
        data = other.data;
        referenceCount = other.referenceCount;
        if (referenceCount) (*referenceCount)++;
        return *this;
    }
    SharedPtrArray& operator=(SharedPtrArray&& other) noexcept
    {
        if (this == &other) return *this;
        ReleaseRef();
        data = other.data;
        referenceCount = other.referenceCount;
        other.data = nullptr;
        other.referenceCount = nullptr;
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
    std::size_t UseCount() const noexcept
    {
        return referenceCount ? *referenceCount : 0;
    }
    ~SharedPtrArray() 
    { 
        ReleaseRef(); 
    }
};

template <typename T>
SharedPtrArray<T> MakeSharedArray(std::size_t n)
{
    return SharedPtrArray<T>(new T[n]());
}