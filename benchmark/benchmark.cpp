#include <benchmark/benchmark.h>
#include <XORList.hpp>
#include <list>

#include <iostream>
#include <fstream>

static constexpr auto ITERATION_COUNTS = 100000;

// Benchmark for XORList push_back
static void BM_XORList_PushBack(benchmark::State &state)
{
    for (auto _ : state)
    {
        scc::XORList<int> list;
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.push_back(i);
        }
    }
}
BENCHMARK(BM_XORList_PushBack);

// Benchmark for std::list push_back
static void BM_StdList_PushBack(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::list<int> list;
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.push_back(i);
        }
    }
}
BENCHMARK(BM_StdList_PushBack);

// Benchmark for XORList pop_back
static void BM_XORList_PopBack(benchmark::State &state)
{
    scc::XORList<int> list;

    for (auto _ : state)
    {
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.push_back(i);
        }
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.pop_back();
        }
    }
}
BENCHMARK(BM_XORList_PopBack);

// Benchmark for std::list pop_back
static void BM_StdList_PopBack(benchmark::State &state)
{
    std::list<int> list;

    for (auto _ : state)
    {
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.push_back(i);
        }
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.pop_back();
        }
    }
}
BENCHMARK(BM_StdList_PopBack);

// Benchmark for XORList insert at beginning
static void
BM_XORList_InsertBegin(benchmark::State &state)
{
    for (auto _ : state)
    {
        scc::XORList<int> list;
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.insert(0, i);
        }
    }
}
BENCHMARK(BM_XORList_InsertBegin);

// Benchmark for std::list insert at beginning
static void BM_StdList_InsertBegin(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::list<int> list;
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.insert(list.begin(), i);
        }
    }
}
BENCHMARK(BM_StdList_InsertBegin);

// Benchmark for XORList erase from beginning
static void BM_XORList_EraseBegin(benchmark::State &state)
{
    scc::XORList<int> list;
    for (int i = 0; i < ITERATION_COUNTS; ++i)
    {
        list.insert(0, i);
    }
    for (auto _ : state)
    {
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.erase(0);
        }
    }
}
BENCHMARK(BM_XORList_EraseBegin);

// Benchmark for std::list erase from beginning
static void BM_StdList_EraseBegin(benchmark::State &state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.insert(list.begin(), i);
        }
        for (int i = 0; i < ITERATION_COUNTS; ++i)
        {
            list.erase(list.begin());
        }
    }
}
BENCHMARK(BM_StdList_EraseBegin);

BENCHMARK_MAIN();
