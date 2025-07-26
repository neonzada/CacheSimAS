#include "utils.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

std::optional<Cache::Config> Utils::parseCacheConfig(const std::string& configStr) {
    std::vector<std::string> tokens;
    std::stringstream ss(configStr);
    std::string token;

    while (std::getline(ss, token, ':'))
        tokens.push_back(token);

    // TODO: melhorar esse error handling porco aq
    if(tokens.size() != 3){
        std::cerr << "Invalid config format: '" << configStr << "'. Expected <n_sets>:<block_size>:<assoc>:<repl>. Skipping.\n";
        return std::nullopt;
    }

    try{
        Cache::Config config;
        config.size = std::stoi(tokens[0]);
        config.block_size = std::stoi(tokens[1]);
        config.associativity = std::stoi(tokens[2]);
        return config;
    }catch(...){
        std::cerr << "Invalid numeric value in config '" << configStr << "'. Skipping.\n";
        return std::nullopt;
    }
}

std::vector<uint32_t> parseFile(const std::string& filename){
    std::ifstream arquivoLeitura("input.bin", std::ios::binary);
}