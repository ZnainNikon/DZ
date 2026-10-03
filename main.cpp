#include <chrono>
#include <iostream>
#include <memory>
#include <iomanip>
#include "array.hpp"
#include "unique.hpp"
#include "shared.hpp"

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

struct Row {int n; double raw; double StdUnique; double MyUnique; double StdShared; double MyShared;};

Row run_benchmark(int N)
{
    Row r {};
    r.n = N;

    r.raw = measure_ms([N] {
        MyVector<int*> v;
        for (int i = 0; i < N; ++i) v.push_back(new int(i));
        for (std::size_t i = 0; i < v.size(); ++i) delete v[i];
    });

    r.StdUnique = measure_ms([N] {
        MyVector<std::unique_ptr<int>> v;
        for (int i = 0; i < N; ++i) v.push_back(std::make_unique<int>(i));
    });

    r.MyUnique = measure_ms([N] {
        MyVector<UniquePtr<int>> v;
        for (int i = 0; i < N; ++i) v.push_back(MakeUnique<int>(i));
    });

    r.StdShared = measure_ms([N] {
        MyVector<std::shared_ptr<int>> v;
        for (int i = 0; i < N; ++i) v.push_back(std::make_shared<int>(i));
    });

    r.MyShared = measure_ms([N] {
        MyVector<SharedPtr<int>> v;
        for (int i = 0; i < N; ++i) v.push_back(MakeShared<int>(i));
    });

    return r;
}

void print_header()
{
    std::cout << "| N | raw | std::unique_ptr | my UniquePtr | std::shared_ptr | my SharedPtr |\n";
    std::cout << "|---|-----|-----------------|--------------|-----------------|--------------|\n";
}

void print_row(const Row& r)
{
    std::cout << "| " << r.n
              << " | " << std::fixed << std::setprecision(4) << r.raw
              << " | " << r.StdUnique
              << " | " << r.MyUnique
              << " | " << r.StdShared
              << " | " << r.MyShared
              << " |\n";
}

int main()
{
    print_header();

    for (int N : {1000, 10000, 100000, 1000000})
    {
        Row r = run_benchmark(N);
        print_row(r);
    }

    return 0;
}