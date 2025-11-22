# hpc-microbenchmark

C++20 benchmark suite for validating new HPC compute nodes before adding them to a cluster. Measures memory bandwidth, cache hierarchy latency, peak FLOPS, and NUMA topology. Outputs a plain table and optional JSON for automated comparison.

## What it measures

| Benchmark | Method | Typical result |
|---|---|---|
| Memory bandwidth | STREAM triad kernel | 50-300 GB/s |
| L1 cache latency | Pointer-chase random walk | 1-4 ns |
| L2 cache latency | Pointer-chase random walk | 4-12 ns |
| L3 cache latency | Pointer-chase random walk | 20-50 ns |
| DRAM latency | Pointer-chase random walk | 60-120 ns |
| CPU GFLOPS (serial) | FMA loop, single core | 2-8 GFLOPS |
| NUMA node count | /sys/devices/system/node | varies |

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

Requires: CMake 3.20+, GCC 12+ or Clang 16+

## Run

```bash
./build/hpc-bench

# Export results to JSON for automated node acceptance testing
./build/hpc-bench --json results/node01.json --array-mb 1024
```

## Sample output

```
Benchmark                            Value         Unit      Notes
--------------------------------------------------------------------------------
numa_node_count                      2.00          nodes     from /sys/devices/system/node
cpu_count                            128.00        cores     logical processors
total_memory_gb                      503.72        GB        from /proc/meminfo
memory_bandwidth_triad               287.43        GB/s      STREAM triad kernel, array=512MB
latency_L1_cache                     1.23          ns        pointer-chase random walk
latency_L2_cache                     4.87          ns        pointer-chase random walk
latency_L3_cache                     31.50         ns        pointer-chase random walk
latency_DRAM                         89.20         ns        pointer-chase random walk
cpu_peak_gflops_serial               6.74          GFLOPS    serial FMA loop
```

## Use in node acceptance testing

Run before adding a node to the cluster. Compare the JSON output to a baseline:

```bash
./build/hpc-bench --json results/$(hostname).json
python3 scripts/compare.py results/baseline.json results/$(hostname).json
```

## Tech

C++20 | CMake | OpenMP (optional) | /proc | /sys
