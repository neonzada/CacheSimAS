#include <iostream>
#include <vector>
#include <sstream>
#include "CacheSimulator.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    std::vector<Cache::Config> cacheConfigs{};
    
    // TODO: find some better way to check this (if there is any)
    // stupid ass way of checking if there is an input file: if the first char of last arg is digit, its probably not a file :)
    if(isdigit(argv[argc-1][0])){
        std::cerr << "Invalid or no input file specified. Aborting...";
        return EXIT_FAILURE;
    }
    if(argc > 5 || argc < 2){
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

    CacheSimulator cacheSimulator(simConfig);

    // TODO: this needs to be read from the inputFile (as a binary file...?)
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