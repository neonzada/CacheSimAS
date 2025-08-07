#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>
#include "CacheSimulator.h"
#include "utils.h"

//#define DEBUG_MODE

int main(int argc, char* argv[]) {

#ifdef DEBUG_MODE
	std::cout << "argc=" << argc << std::endl;
	for(int i = 0; i < argc; i++){
		std::cout << "argv["<< i << "]" << argv[i] << std::endl;
	}
#endif

	if(argc < 2){
		std::cerr << "Too few arguments: expected 1-3 cacheConfigs and 1 input file.\n";
		return EXIT_FAILURE;
	}
    
	std::string inputFile = argv[argc-1];
	std::optional<std::vector<uint32_t>> addresses = Utils::readBinaryAddresses(inputFile);
	if(!addresses){
		std::cerr << "Invalid or no input file specified. Aborting...";
        return EXIT_FAILURE;
	}

	std::vector<Cache::Config> cacheConfigs{};

	bool wellDef = false;
	for(int i = 1; i < argc-1; i++){
		auto parsed = Utils::parseCacheConfig(argv[i]);
		if(parsed){
			cacheConfigs.push_back(*parsed);
			wellDef = true;
		}
	}
	if(!wellDef){
		std::cout << "No well defined config specified. Using default value for L1 (1024:4:1:r)\n";
		cacheConfigs.push_back({1024, 4, 1, Cache::RANDOM});
	}
    
    std::cout << "Using input file: " << inputFile << std::endl;

    CacheSimulator::SimulatorConfig simConfig{};
    simConfig.cacheConfigs = cacheConfigs;
	CacheSimulator cacheSimulator(simConfig);

#ifdef DEBUG_MODE
    int count = 0;
    for(const auto& cfg : simConfig.cacheConfigs){
        std::cout << "L" << ++count << "=" << cfg.size << ":" << cfg.block_size << ":" << cfg.associativity << ":" << cfg.policy << std::endl;
    }
#endif
    
        
#ifdef DEBUG_MODE
	for(uint32_t addr : *addresses){
		std::cout << addr << std::endl;
	}
#endif
	
	std::cout << "=== Simulating Access... ===" << std::endl;
	for (uint32_t address : *addresses) {
		bool hit = cacheSimulator.simulateRead(address);
		std::cout << "[" << address << "]\t" << (hit ? "Hit" : "Miss") << "\n";
	}
	cacheSimulator.printReport();
    return 0;
}