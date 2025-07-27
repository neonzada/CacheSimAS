#include <iostream>
#include <vector>
#include <sstream>
#include "CacheSimulator.h"
#include "utils.h"

#define DEBUG_MODE

int main(int argc, char* argv[]) {
    std::vector<Cache::Config> cacheConfigs{};

    // if the first char of last arg is digit, its probably not a file :)
    if(isdigit(argv[argc-1][0])){
        std::cerr << "Invalid or no input file specified. Aborting...";
        return EXIT_FAILURE;
    }
    if(argc > 5 || argc < 3){
        std::cout << "Too many/few arguments: expected 1-3 cacheConfigs and 1 input file.\n";
        std::cout << "Using default value (1024:4:1:r)\n";
        cacheConfigs.push_back({1024, 4, 1, Cache::RANDOM});
    }else{
        for(int i = 1; i < argc - 1; ++i){
            auto parsed = Utils::parseCacheConfig(argv[i]);
            if(parsed) cacheConfigs.push_back(*parsed);
        }
    }
    
    std::string inputFile = argv[argc - 1];
    std::cout << "Using input file: " << inputFile << std::endl;

    CacheSimulator::SimulatorConfig simConfig{};
    simConfig.cacheConfigs = cacheConfigs;

    #ifdef DEBUG_MODE
    int count = 0;
    for(const auto& cfg : simConfig.cacheConfigs){
        std::cout << "L" << ++count << "=" << cfg.size << ":" << cfg.block_size << ":" << cfg.associativity << ":" << cfg.policy << std::endl;
    }
    CacheSimulator cacheSimulator(simConfig);
    #endif
    
    if(std::optional<std::vector<uint32_t>> addresses = Utils::readBinaryAddresses(inputFile)){        
        std::cout << "=== Simulate Access... ===";
        for (uint32_t address : *addresses) {
            bool hit = cacheSimulator.simulateRead(address);
            std::cout << (hit ? "Hit" : "Miss") << "\n";
        }
        cacheSimulator.printReport();
    }else{
        std::cerr << "Error: No addresses loaded.\n"; //this technically should never occur unless we're not reading it properly
        return EXIT_FAILURE;
    }
    return 0;
}