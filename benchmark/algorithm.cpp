#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

static constexpr auto ITERATION_COUNTS = 100000;

// Benchmark for XORList splice
static void BM_XORList_Splice(benchmark::State &state)
{
    scc::XORList<int> list1, list2;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list1.push_back(i);
    }
    for (auto _ : state)
    {
        list2.splice(0, list1);
    }
}
BENCHMARK(BM_XORList_Splice);

// Benchmark for std::list splice
static void BM_StdList_Splice(benchmark::State &state)
{
    std::list<int> list1, list2;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list1.push_back(i);
    }
    for (auto _ : state)
    {
        list2.splice(list2.begin(), list1);
    }
}
BENCHMARK(BM_StdList_Splice);

// Benchmark for XORList merge
static void BM_XORList_Merge(benchmark::State &state)
{
    scc::XORList<int> list1, list2;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list1.push_back(i * 2);
        list2.push_back(i * 2 + 1);
    }
    for (auto _ : state)
    {
        list1.merge(list2);
    }
}
BENCHMARK(BM_XORList_Merge);

// Benchmark for std::list merge
static void BM_StdList_Merge(benchmark::State &state)
{
    std::list<int> list1, list2;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list1.push_back(i * 2);
        list2.push_back(i * 2 + 1);
    }
    for (auto _ : state)
    {
        list1.merge(list2);
    }
}
BENCHMARK(BM_StdList_Merge);

BENCHMARK_MAIN();
