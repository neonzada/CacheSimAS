#pragma once
#include "Cache.h"
#include <string>

class CacheSimulator {

public:
    struct SimulatorConfig {
        std::vector<Cache::Config> cacheConfigs;
    };

private:
    std::vector<Cache> caches;
    std::vector<std::string> cacheNames;
    SimulatorConfig simConfig;
    uint64_t total_memory_accesses = 0;

public:
    explicit CacheSimulator(const SimulatorConfig& config);

    bool simulateRead(uint32_t address);

    void resetSimulation();

    void printReport() const;

    const Cache& get_cache() const;
    const Cache::Stats& get_stats() const;
};