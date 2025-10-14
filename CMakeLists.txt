cmake_minimum_required(VERSION 3.20)
project(hpc-microbenchmark VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Release with full optimisation by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops -ffast-math")

# Optional: OpenMP for parallel bandwidth test
find_package(OpenMP)
find_package(Threads REQUIRED)

add_executable(hpc-bench
    src/main.cpp
    src/mem_bandwidth.cpp
    src/cache_latency.cpp
    src/numa_topology.cpp
    src/cpu_flops.cpp
    src/reporter.cpp
)

target_include_directories(hpc-bench PRIVATE include)
target_link_libraries(hpc-bench PRIVATE Threads::Threads)

if(OpenMP_CXX_FOUND)
    target_link_libraries(hpc-bench PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(hpc-bench PRIVATE HPC_BENCH_OMP)
endif()
