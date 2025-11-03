#include "bench.hpp"
#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace bench {

// Pointer-chase random walk: eliminates prefetcher, forces true latency.
static double chase_ns(std::size_t size_bytes) {
    std::size_t n     = size_bytes / sizeof(std::size_t);
    std::vector<std::size_t> buf(n);

    // Build a random permutation (every element points to next in random order)
    std::iota(buf.begin(), buf.end(), 0);
    std::mt19937_64 rng(42);
    std::shuffle(buf.begin(), buf.end(), rng);

    const int iters = 1 << 26;  // ~64M chases
    std::size_t idx = 0;
    Timer tm; tm.reset();
    for (int i = 0; i < iters; ++i)
        idx = buf[idx % n];
    double elapsed = tm.elapsed_s();
    (void)idx;  // prevent optimisation

    return (elapsed / iters) * 1e9;  // nanoseconds
}

std::vector<BenchResult> run_cache_latency() {
    struct Level { std::string name; std::size_t bytes; };
    std::vector<Level> levels = {
        {"L1_cache",  32ULL   * 1024},
        {"L2_cache",  256ULL  * 1024},
        {"L3_cache",  8ULL    * 1024 * 1024},
        {"DRAM",      256ULL  * 1024 * 1024},
    };

    std::vector<BenchResult> out;
    for (auto& [name, sz] : levels) {
        out.push_back({
            .name  = "latency_" + name,
            .value = chase_ns(sz),
            .unit  = "ns",
            .note  = "pointer-chase random walk",
        });
    }
    return out;
}

} // namespace bench
