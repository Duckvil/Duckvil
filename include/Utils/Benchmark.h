#pragma once

#include <chrono>

#define DUCKVIL_BENCHMARK_BEGIN(name) auto& name = std::chrono::high_resolution_clock::now()
#define DUCKVIL_BENCHMARK_END(name) auto& name = std::chrono::high_resolution_clock::now()
#define DUCKVIL_BENCHMARK_DURATION(res, begin, end) auto& res = std::chrono::duration_cast<std::chrono::microseconds>(end - begin)