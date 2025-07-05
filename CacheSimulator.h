#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H
#include "Cache.h"

class CacheSimulator {

public:
    struct SimulatorConfig {
        Cache::Config cacheConfig;
    };

private:
    Cache cache;
    SimulatorConfig simConfig;

public:
    explicit CacheSimulator(const SimulatorConfig& config);

    bool simulateRead(uint32_t address);

    void resetSimulation();

    void printReport() const;

    const Cache& get_cache() const;
    const Cache::Stats& get_stats() const;
};

#endif
