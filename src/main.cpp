#include "bench.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string json_out;
    std::size_t array_mb = 512;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--json"      && i + 1 < argc) json_out  = argv[++i];
        if (arg == "--array-mb"  && i + 1 < argc) array_mb  = std::stoul(argv[++i]);
        if (arg == "--help") {
            std::cout << "Usage: hpc-bench [--json output.json] [--array-mb N]\n";
            return 0;
        }
    }

    std::cout << "HPC Microbenchmark Suite\n";
    std::cout << "========================\n";

    std::vector<bench::BenchResult> all;

    std::cout << "\nRunning NUMA topology scan...\n";
    auto topo = bench::run_numa_topology();
    all.insert(all.end(), topo.begin(), topo.end());

    std::cout << "Running memory bandwidth (triad, " << array_mb << "MB)...\n";
    auto bw = bench::run_mem_bandwidth(array_mb);
    all.insert(all.end(), bw.begin(), bw.end());

    std::cout << "Running cache hierarchy latency...\n";
    auto cl = bench::run_cache_latency();
    all.insert(all.end(), cl.begin(), cl.end());

    std::cout << "Running CPU FLOPS estimate...\n";
    auto fp = bench::run_cpu_flops();
    all.insert(all.end(), fp.begin(), fp.end());

    bench::report(all, json_out);
    return 0;
}
