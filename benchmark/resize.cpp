#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

static constexpr auto ITERATION_COUNTS = 100000;

// Benchmark for XORList resize
static void BM_XORList_Resize(benchmark::State &state)
{
    scc::XORList<int> list;
    for (auto _ : state)
    {
        list.resize(ITERATION_COUNTS, 42);
    }
}
BENCHMARK(BM_XORList_Resize);

// Benchmark for std::list resize
static void BM_StdList_Resize(benchmark::State &state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        list.resize(ITERATION_COUNTS, 42);
    }
}
BENCHMARK(BM_StdList_Resize);

BENCHMARK_MAIN();
