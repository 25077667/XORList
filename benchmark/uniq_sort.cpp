#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

static constexpr auto ITERATION_COUNTS = 100000;
static constexpr auto SORT_COUNTS = 1000;

// Benchmark for XORList unique
static void BM_XORList_Unique(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i % 10);
    }
    for (auto _ : state)
    {
        list.unique();
    }
}
BENCHMARK(BM_XORList_Unique);

// Benchmark for std::list unique
static void BM_StdList_Unique(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i % 10);
    }
    for (auto _ : state)
    {
        list.unique();
    }
}
BENCHMARK(BM_StdList_Unique);

// Benchmark for XORList sort
static void BM_XORList_Sort(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = SORT_COUNTS; i > 0; --i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.sort();
    }
}
BENCHMARK(BM_XORList_Sort);

// Benchmark for std::list sort
static void BM_StdList_Sort(benchmark::State &state)
{
    std::list<int> list;
    for (int i = SORT_COUNTS; i > 0; --i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.sort();
    }
}
BENCHMARK(BM_StdList_Sort);

BENCHMARK_MAIN();
