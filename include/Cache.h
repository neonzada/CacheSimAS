#pragma once
#include <cstdint>
#include <map>
#include <vector>

class Memory;

class Cache {
public:
    enum ReplacementPolicy { LRU, FIFO, RANDOM };

    struct Config {
        int size;
        int block_size;
        int associativity;
        ReplacementPolicy policy;
    };

    struct Stats {
        uint64_t accesses = 0;
        uint64_t hits = 0;
        uint64_t misses = 0;
        uint64_t evictions = 0;
    } stats;

private:
    Config config;

    int num_sets;
    struct Block {
        bool valid = false;
        uint32_t tag = 0;
        uint32_t last_used = 0;
    };


    std::vector<std::vector<Block>> sets;
    uint64_t global_timestamp = 0;

    int getSetIndex(uint32_t address) const;
    uint32_t getTag(uint32_t address) const;

    int findBlock(int set_index, uint32_t tag) const;
    int chooseBlockToEvict(int set_index);

public:
    explicit Cache(const Config& config);

    bool read(uint32_t address);
    const Stats& getStats() const { return stats; }
};