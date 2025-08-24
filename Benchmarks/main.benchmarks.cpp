#include <benchmark/benchmark.h>
#include "CoreRuntime.h"

static void Benchmark_Example([[maybe_unused]] benchmark::State& state)
{
    for ([[maybe_unused]] const auto& _ : state)
    {
        benchmark::DoNotOptimize(1 + 1);
    }
}

BENCHMARK(Benchmark_Example);


int main(int argc, char** argv)
{
    Runtime::CoreRuntime runtime;

    benchmark::MaybeReenterWithoutASLR(argc, argv);
    char arg0_default[] = "benchmark";
    char* args_default = reinterpret_cast<char*>(arg0_default);
    if (!argv)
    {
        argc = 1;
        argv = &args_default;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
