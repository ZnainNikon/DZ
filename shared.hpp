#include <stdexcept>
#include <utility>
#include <cstddef>

template <typename T>
class SharedPtr
{
    private:
    T* data;
    std::size_t* referenceCount;
    void ReleaseRef() noexcept
    {
        if (referenceCount && --(*referenceCount) == 0)
        {
            delete data;
            delete referenceCount;
        }
        data = nullptr;
        referenceCount = nullptr;
    }

    public:
    SharedPtr() noexcept : data(nullptr), referenceCount(nullptr) {}
    SharedPtr(T* data) : data(data), referenceCount(new std::size_t(1)) {}
    SharedPtr(const SharedPtr& other) : data(other.data), referenceCount(other.referenceCount)
    {
        if (referenceCount) (*referenceCount)++;
    }
    SharedPtr(SharedPtr&& other) noexcept : data(other.data), referenceCount(other.referenceCount)
    {
        other.data = nullptr;
        other.referenceCount = nullptr;
    }
    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this == &other) return *this;
        ReleaseRef();
        data = other.data;
        referenceCount = other.referenceCount;
        if (referenceCount) (*referenceCount)++;
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        if (this == &other) return *this;
        ReleaseRef();
        data = other.data;
        referenceCount = other.referenceCount;
        other.data = nullptr;
        other.referenceCount = nullptr;
        return *this;
    }
    T& operator*() const
    {
        if (!data) throw std::runtime_error("Data is void!");
        return *data;
    }
    T* operator->() const noexcept 
    { 
        return data; 
    }
    T* Get() const noexcept 
    { 
        return data; 
    }
    std::size_t UseCount() const noexcept 
    { 
        return referenceCount ? *referenceCount : 0; 
    }
    ~SharedPtr() 
    { 
        ReleaseRef(); 
    }
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class SharedPtr<T[]>
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
    SharedPtr() noexcept : data(nullptr), referenceCount(nullptr) {}
    SharedPtr(T* data) : data(data), referenceCount(new std::size_t(1)) {}
    SharedPtr(const SharedPtr& other) : data(other.data), referenceCount(other.referenceCount)
    {
        if (referenceCount) (*referenceCount)++;
    }
    SharedPtr(SharedPtr&& other) noexcept : data(other.data), referenceCount(other.referenceCount)
    {
        other.data = nullptr;
        other.referenceCount = nullptr;
    }
    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this == &other) return *this;
        ReleaseRef();
        data = other.data;
        referenceCount = other.referenceCount;
        if (referenceCount) (*referenceCount)++;
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept
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

    ~SharedPtr() 
    { 
        ReleaseRef(); 
    }
};


template <typename T>
SharedPtr<T[]> MakeSharedArray(std::size_t n)
{
    return SharedPtr<T[]>(new T[n]());
}