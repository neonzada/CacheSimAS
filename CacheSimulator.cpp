#include "CacheSimulator.h"
#include <iostream>

CacheSimulator::CacheSimulator(const SimulatorConfig& config) : cache(config.cacheConfig), simConfig(config) {}

bool CacheSimulator::simulateRead(uint32_t address) {
    return cache.read(address);
}

void CacheSimulator::resetSimulation() {
    cache = Cache(simConfig.cacheConfig);
}

void CacheSimulator::printReport() const {
    const auto& stats = cache.getStats();

    double hitRate = 0.0;
    if (stats.accesses > 0) {
        hitRate = static_cast<double>(stats.hits) / stats.accesses * 100.0;
    }

    std::cout << "\n=== Cache Simulation Report ===\n";
    std::cout << "Total Accesses: " << stats.accesses << "\n";
    std::cout << "Hits: "           << stats.hits << "\n";
    std::cout << "Misses: "         << stats.misses << "\n";
    std::cout << "Evictions: "      << stats.evictions << "\n";
    std::cout << "Hit Rate: "       << hitRate << "\n";
    std::cout << "====================================\n";
}

