#include "CacheSimulator.h"
#include <iostream>

CacheSimulator::CacheSimulator(const SimulatorConfig& config) : simConfig(config), total_memory_accesses(0) {
    for(const auto& cfg : config.cacheConfigs){
        caches.emplace_back(cfg);
    }
}

bool CacheSimulator::simulateRead(uint32_t address) {
    for(auto cache : caches){
        if(cache.read(address)){
            return true;
        }
    }
    total_memory_accesses++;
    return false;
}

void CacheSimulator::resetSimulation() {
    caches.clear();
    for(const auto& cfg: simConfig.cacheConfigs){
        caches.emplace_back(cfg);
    }
    total_memory_accesses = 0;
}

void CacheSimulator::printReport() const {
    std::cout << "\n=== Cache Simulation Report ===\n";

    for(size_t i = 0; i < caches.size(); ++i){
        double hitRate = 0.0;
        const auto& stats = caches[i].getStats();
        std::cout << "Cache L" << (i+1) << ":\n";
        std::cout << "Total Accesses: " << stats.accesses << "\n";
        std::cout << "Hits: "           << stats.hits << "\n";
        std::cout << "Misses: "         << stats.misses << "\n";
        std::cout << "Evictions: "      << stats.evictions << "\n";
        std::cout << "Hit Rate: "       << hitRate << "\n";
        std::cout << "====================================\n";
    }
    std::cout << "Total Memory Access (Miss Total): " << total_memory_accesses << "\n";
    std::cout << "====================================\n";
    double hitRate = 0.0;
    // if (stats.accesses > 0) {
    //     hitRate = static_cast<double>(stats.hits) / stats.accesses * 100.0;

}