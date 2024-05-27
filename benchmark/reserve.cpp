#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

static constexpr auto ITERATION_COUNTS = 100000;

// Benchmark for XORList reverse
static void BM_XORList_Reverse(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.reverse();
    }
}
BENCHMARK(BM_XORList_Reverse);

// Benchmark for std::list reverse
static void BM_StdList_Reverse(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.reverse();
    }
}
BENCHMARK(BM_StdList_Reverse);

BENCHMARK_MAIN();
