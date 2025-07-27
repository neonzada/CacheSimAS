#include "utils.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

std::optional<std::vector<uint32_t>> Utils::readBinaryAddresses(const std::string& filename) {
    std::vector<uint32_t> outAddresses;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error reading file '" << filename << "'" << std::endl;
        return std::nullopt;
    }

    uint32_t addr;
    while (file.read(reinterpret_cast<char*>(&addr), sizeof(addr)))
        outAddresses.push_back(addr);

    for(auto addresses : outAddresses){
        std::cout << addresses << std::endl;
    }
    if (file.bad()) {
        std::cerr << "Error during reading file '" << filename << "'" << std::endl;
        return std::nullopt;
    }
    return outAddresses;
}

std::optional<Cache::Config> Utils::parseCacheConfig(const std::string& configStr) {
    std::vector<std::string> tokens;
    std::stringstream ss(configStr);
    std::string token;

    while (std::getline(ss, token, ':'))
        tokens.push_back(token);

    // TODO: melhorar esse error handling porco aq
    if(tokens.size() != 4){
        std::cerr << "Invalid config format: '" << configStr << "'. Expected <n_sets>:<block_size>:<assoc>:<repl>. Skipping.\n";
        return std::nullopt;
    }

    try{
        Cache::Config config;
        config.size = std::stoi(tokens[0]);
        config.block_size = std::stoi(tokens[1]);
        config.associativity = std::stoi(tokens[2]);
        config.policy = tokens[3] == "l" ? Cache::LRU
                      : tokens[3] == "f" ? Cache::FIFO
                      : Cache::RANDOM;

        return config;
    }catch(...){
        std::cerr << "Invalid value in config '" << configStr << "'. Skipping.\n";
        return std::nullopt;
    }
}

std::vector<uint32_t> parseFile(const std::string& filename){
    std::ifstream arquivoLeitura("input.bin", std::ios::binary);
}