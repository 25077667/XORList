#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

static constexpr auto ITERATION_COUNTS = 100000;

// Benchmark for XORList size
static void BM_XORList_Size(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.size());
    }
}
BENCHMARK(BM_XORList_Size);

// Benchmark for std::list size
static void BM_StdList_Size(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.size());
    }
}
BENCHMARK(BM_StdList_Size);

// Benchmark for XORList empty
static void BM_XORList_Empty(benchmark::State &state)
{
    scc::XORList<int> list;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.empty());
    }
}
BENCHMARK(BM_XORList_Empty);

// Benchmark for std::list empty
static void BM_StdList_Empty(benchmark::State &state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.empty());
    }
}
BENCHMARK(BM_StdList_Empty);

// Benchmark for XORList front
static void BM_XORList_Front(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.front());
    }
}
BENCHMARK(BM_XORList_Front);

// Benchmark for std::list front
static void BM_StdList_Front(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.front());
    }
}
BENCHMARK(BM_StdList_Front);

// Benchmark for XORList back
static void BM_XORList_Back(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.back());
    }
}
BENCHMARK(BM_XORList_Back);

// Benchmark for std::list back
static void BM_StdList_Back(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.back());
    }
}
BENCHMARK(BM_StdList_Back);

// Benchmark for XORList clear
static void BM_XORList_Clear(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.clear();
    }
}
BENCHMARK(BM_XORList_Clear);

// Benchmark for std::list clear
static void BM_StdList_Clear(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        list.clear();
    }
}
BENCHMARK(BM_StdList_Clear);

// Benchmark for XORList begin
static void BM_XORList_Begin(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.begin());
    }
}
BENCHMARK(BM_XORList_Begin);

// Benchmark for std::list begin
static void BM_StdList_Begin(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.begin());
    }
}
BENCHMARK(BM_StdList_Begin);

// Benchmark for XORList cbegin
static void BM_XORList_CBegin(benchmark::State &state)
{
    const scc::XORList<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.cbegin());
    }
}
BENCHMARK(BM_XORList_CBegin);

// Benchmark for std::list cbegin
static void BM_StdList_CBegin(benchmark::State &state)
{
    const std::list<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.cbegin());
    }
}
BENCHMARK(BM_StdList_CBegin);

// Benchmark for XORList end
static void BM_XORList_End(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.end());
    }
}
BENCHMARK(BM_XORList_End);

// Benchmark for std::list end
static void BM_StdList_End(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.end());
    }
}
BENCHMARK(BM_StdList_End);

// Benchmark for XORList cend
static void BM_XORList_CEnd(benchmark::State &state)
{
    const scc::XORList<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.cend());
    }
}
BENCHMARK(BM_XORList_CEnd);

// Benchmark for std::list cend
static void BM_StdList_CEnd(benchmark::State &state)
{
    const std::list<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.cend());
    }
}
BENCHMARK(BM_StdList_CEnd);

// Benchmark for XORList rbegin
static void BM_XORList_RBegin(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.rbegin());
    }
}
BENCHMARK(BM_XORList_RBegin);

// Benchmark for std::list rbegin
static void BM_StdList_RBegin(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.rbegin());
    }
}
BENCHMARK(BM_StdList_RBegin);

// Benchmark for XORList crbegin
static void BM_XORList_CRBegin(benchmark::State &state)
{
    const scc::XORList<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.crbegin());
    }
}
BENCHMARK(BM_XORList_CRBegin);

// Benchmark for std::list crbegin
static void BM_StdList_CRBegin(benchmark::State &state)
{
    const std::list<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.crbegin());
    }
}
BENCHMARK(BM_StdList_CRBegin);

// Benchmark for XORList rend
static void BM_XORList_REnd(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.rend());
    }
}
BENCHMARK(BM_XORList_REnd);

// Benchmark for std::list rend
static void BM_StdList_REnd(benchmark::State &state)
{
    std::list<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.push_back(i);
    }
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.rend());
    }
}
BENCHMARK(BM_StdList_REnd);

// Benchmark for XORList crend
static void BM_XORList_CREnd(benchmark::State &state)
{
    const scc::XORList<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.crend());
    }
}
BENCHMARK(BM_XORList_CREnd);

// Benchmark for std::list crend
static void BM_StdList_CREnd(benchmark::State &state)
{
    const std::list<int> list(ITERATION_COUNTS, 42);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.crend());
    }
}
BENCHMARK(BM_StdList_CREnd);

// Benchmark for XORList max_size
static void BM_XORList_MaxSize(benchmark::State &state)
{
    scc::XORList<int> list;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.max_size());
    }
}
BENCHMARK(BM_XORList_MaxSize);

// Benchmark for std::list max_size
static void BM_StdList_MaxSize(benchmark::State &state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(list.max_size());
    }
}
BENCHMARK(BM_StdList_MaxSize);

BENCHMARK_MAIN();
