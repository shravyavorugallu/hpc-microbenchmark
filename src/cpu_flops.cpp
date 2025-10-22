#include "bench.hpp"
#include <cmath>
#include <vector>

namespace bench {

// Double-precision FMA loop: 2 FLOPS per iteration.
std::vector<BenchResult> run_cpu_flops() {
    const std::size_t iters = 1ULL << 28;  // 256M iterations
    volatile double acc = 1.0;
    const  double  a   = 1.000000001;
    const  double  b   = 0.999999999;

    Timer tm; tm.reset();
    for (std::size_t i = 0; i < iters; ++i)
        acc = acc * a + b;   // 1 FMA
    double elapsed = tm.elapsed_s();
    (void)acc;

    double gflops = (2.0 * iters) / elapsed / 1e9;
    return {{
        .name  = "cpu_peak_gflops_serial",
        .value = gflops,
        .unit  = "GFLOPS",
        .note  = "serial FMA loop (single core, no SIMD auto-vectorisation guard)",
    }};
}

} // namespace bench
