#include "bench.hpp"
#include <cstring>
#include <numeric>
#include <vector>

namespace bench {

// STREAM-style triad: C[i] = A[i] + scalar * B[i]
// Measures sustained memory bandwidth (not peak cache bandwidth).
static double triad_gb_s(std::size_t n, int trials = 5) {
    const double scalar = 3.0;
    std::vector<double> A(n, 1.0), B(n, 2.0), C(n, 0.0);

    // Warm up
    for (std::size_t i = 0; i < n; ++i) C[i] = A[i] + scalar * B[i];

    double best = 0.0;
    for (int t = 0; t < trials; ++t) {
        Timer tm; tm.reset();
        for (std::size_t i = 0; i < n; ++i) C[i] = A[i] + scalar * B[i];
        double elapsed = tm.elapsed_s();
        // 3 arrays read/written: 3 * n * 8 bytes
        double gb = 3.0 * static_cast<double>(n) * sizeof(double) / 1e9;
        best = std::max(best, gb / elapsed);
    }
    return best;
}

std::vector<BenchResult> run_mem_bandwidth(std::size_t array_mb) {
    std::size_t n = (array_mb * 1024ULL * 1024ULL) / sizeof(double);
    double bw = triad_gb_s(n);
    return {{
        .name  = "memory_bandwidth_triad",
        .value = bw,
        .unit  = "GB/s",
        .note  = "STREAM triad kernel, array=" + std::to_string(array_mb) + "MB per buffer",
    }};
}

} // namespace bench
