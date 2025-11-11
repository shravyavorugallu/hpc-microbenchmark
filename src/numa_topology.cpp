#include "bench.hpp"
#include <fstream>
#include <sstream>
#include <string>

namespace bench {

// Read NUMA info from sysfs (Linux only).
std::vector<BenchResult> run_numa_topology() {
    std::vector<BenchResult> out;

    // Count NUMA nodes
    int nodes = 0;
    for (int n = 0; n < 64; ++n) {
        std::ifstream f("/sys/devices/system/node/node" + std::to_string(n) + "/cpulist");
        if (!f.good()) break;
        ++nodes;
    }
    out.push_back({.name="numa_node_count", .value=static_cast<double>(nodes),
                   .unit="nodes", .note="from /sys/devices/system/node"});

    // CPU count
    std::ifstream cpuinfo("/proc/cpuinfo");
    int cpus = 0;
    std::string line;
    while (std::getline(cpuinfo, line))
        if (line.find("processor") == 0) ++cpus;
    out.push_back({.name="cpu_count", .value=static_cast<double>(cpus),
                   .unit="cores", .note="logical processors from /proc/cpuinfo"});

    // Total memory (MB)
    std::ifstream meminfo("/proc/meminfo");
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            std::istringstream ss(line);
            std::string key; long kb;
            ss >> key >> kb;
            out.push_back({.name="total_memory_gb",
                           .value=static_cast<double>(kb) / 1024.0 / 1024.0,
                           .unit="GB", .note="from /proc/meminfo"});
            break;
        }
    }
    return out;
}

} // namespace bench
