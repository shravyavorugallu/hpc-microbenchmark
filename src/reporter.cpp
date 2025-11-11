#include "bench.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

namespace bench {

void report(const std::vector<BenchResult>& results, const std::string& json_path) {
    std::cout << "\n";
    std::cout << std::left
              << std::setw(36) << "Benchmark"
              << std::setw(14) << "Value"
              << std::setw(10) << "Unit"
              << "Notes\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(36) << r.name
                  << std::setw(14) << std::fixed << std::setprecision(2) << r.value
                  << std::setw(10) << r.unit
                  << r.note << "\n";
    }
    std::cout << "\n";

    if (json_path.empty()) return;

    std::ofstream f(json_path);
    f << "[\n";
    for (std::size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        f << "  {\"name\":\"" << r.name << "\","
          << "\"value\":" << std::fixed << std::setprecision(4) << r.value << ","
          << "\"unit\":\"" << r.unit << "\","
          << "\"note\":\"" << r.note << "\"}";
        if (i + 1 < results.size()) f << ",";
        f << "\n";
    }
    f << "]\n";
    std::cout << "Results written to: " << json_path << "\n";
}

} // namespace bench
