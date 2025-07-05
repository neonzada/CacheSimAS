#include <iostream>
#include <vector>
#include "CacheSimulator.h"

int main() {
    Cache::Config cacheConfig{};
    cacheConfig.size = 1024;
    cacheConfig.block_size = 16;
    cacheConfig.associativity = 2;
    cacheConfig.policy = Cache::LRU;

    CacheSimulator::SimulatorConfig simConfig{};
    simConfig.cacheConfig = cacheConfig;

    CacheSimulator cacheSimulator(simConfig);

    std::vector<uint32_t> addresses = {
        0x0000, 0x0010, 0x0020, 0x0030, 0x0000, 0x0010, 0x0100, 0x0200, 0x0030
    };

    std::cout << "=== Simulate Access... ===";
    for (uint32_t address : addresses) {
        bool hit = cacheSimulator.simulateRead(address);
        std::cout << (hit ? "Hit" : "Miss") << "\n";
    }
    cacheSimulator.printReport();
    return 0;
}