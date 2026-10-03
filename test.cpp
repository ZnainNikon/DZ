#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <memory>
#include "array.hpp"
#include "ptrs.cpp"

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

template <typename Fn>
double measure_ms(Fn&& fn)
{
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return Ms(end - start).count();
}

TEST(Benchmark, Raw)
{
    double t = measure_ms([] {
        MyVector<int*> v;
        for (int i = 0; i < 1000; ++i) v.push_back(new int(i));
        for (std::size_t i = 0; i < v.size(); ++i) delete v[i];
    });
    std::cout << "raw: " << t << " ms\n";
}

TEST(Benchmark, StdUnique)
{
    double t = measure_ms([] {
        MyVector<std::unique_ptr<int>> v;
        for (int i = 0; i < 1000; ++i) v.push_back(std::make_unique<int>(i));
    });
    std::cout << "std::unique_ptr: " << t << " ms\n";
}

TEST(Benchmark, MyUnique)
{
    double t = measure_ms([] {
        MyVector<UniquePtr<int>> v;
        for (int i = 0; i < 1000; ++i) v.push_back(MakeUnique<int>(i));
    });
    std::cout << "my UniquePtr: " << t << " ms\n";
}

TEST(Benchmark, StdShared)
{
    double t = measure_ms([] {
        MyVector<std::shared_ptr<int>> v;
        for (int i = 0; i < 1000; ++i) v.push_back(std::make_shared<int>(i));
    });
    std::cout << "std::shared_ptr: " << t << " ms\n";
}

TEST(Benchmark, MyShared)
{
    double t = measure_ms([] {
        MyVector<SharedPtr<int>> v;
        for (int i = 0; i < 1000; ++i) v.push_back(MakeShared<int>(i));
    });
    std::cout << "my SharedPtr: " << t << " ms\n";
}