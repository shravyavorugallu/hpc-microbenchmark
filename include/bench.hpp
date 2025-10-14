#pragma once
#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

namespace bench {

// Wall-clock timer
struct Timer {
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start;
    void reset() { start = Clock::now(); }
    double elapsed_s() const {
        return std::chrono::duration<double>(Clock::now() - start).count();
    }
};

struct BenchResult {
    std::string name;
    double      value;
    std::string unit;
    std::string note;
};

// Memory bandwidth (triad kernel, GB/s)
std::vector<BenchResult> run_mem_bandwidth(std::size_t array_mb = 512);

// Cache hierarchy latency (ns per access, pointer-chase)
std::vector<BenchResult> run_cache_latency();

// Peak FLOPS estimate (double-precision FMA)
std::vector<BenchResult> run_cpu_flops();

// NUMA topology info
std::vector<BenchResult> run_numa_topology();

// Reporter: print table + optional JSON file
void report(const std::vector<BenchResult>& results,
            const std::string& json_path = "");

} // namespace bench
