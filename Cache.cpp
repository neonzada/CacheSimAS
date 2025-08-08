#include "Cache.h"
#include <random>
#include <limits>
#include <iostream>

Cache::Cache(const Config &config) : config(config), stats{}, global_timestamp(0) {
    int blocks_per_set = config.associativity;
    int total_blocks = config.size / config.block_size;
    num_sets = total_blocks / blocks_per_set;
    if (num_sets <= 0)
        throw std::invalid_argument("Number of sets must be greater than 0");
    sets.resize(num_sets, std::vector<Block>(blocks_per_set));
}

int Cache::getSetIndex(const uint32_t address) const {
    return (address / config.block_size) % num_sets;
}

uint32_t Cache::getTag(const uint32_t address) const {
    return address / (config.block_size * num_sets);
}

int Cache::findBlock(int set_index, uint32_t tag) const{
    const auto& set = sets[set_index];
    for (size_t i = 0; i < set.size(); ++i) {
        if (set[i].valid && set[i].tag == tag) {
            return static_cast<int>(i);
        }
    }
    return -1; // Miss
}

int Cache::chooseBlockToEvict(int set_index) {
    auto& set = sets[set_index];
    for (size_t i = 0; i < set.size(); ++i) {
        if (!set[i].valid) {
            return static_cast<int>(i);
        }
    }
    switch (config.policy) {
        case LRU: {
            uint32_t oldest = std::numeric_limits<uint32_t>::max();
            int evict_index = 0;
            for (size_t i = 0; i < set.size(); ++i) {
                if (set[i].last_used < oldest) {
                    oldest = set[i].last_used;
                    evict_index = static_cast<int>(i);
                }
            }
            return evict_index;
        }
        case FIFO: {
            return chooseBlockToEvict(set_index);
        }
        case RANDOM: {
            static std::mt19937 rng(42);
            std::uniform_int_distribution<int> dist(0, static_cast<int>(set.size() - 1));
            return dist(rng);
        }
    }
    return 0;
}

bool Cache::read(uint32_t address) {
    stats.accesses++;
    global_timestamp++;

    int set_index = getSetIndex(address);
    uint32_t tag = getTag(address);

    auto& set = sets[set_index];
    int block_index = findBlock(set_index, tag);

    if (block_index != -1) { //Hit
        stats.hits++;
        set[block_index].last_used = global_timestamp;
        return true;
    }
    stats.total_misses++;
	
	// checking for misses: calculates unique id based on <tag, set_index> pair
	// tag goes in the upper bits of the id
	// bitwise OR puts the set_index in the lower bits, creating a unique id
	uint64_t unique_id = ((uint64_t)tag << static_cast<int>(std::log2(num_sets))) | set_index;
    if (seen_blocks.find(unique_id) == seen_blocks.end()) {
        stats.comp_misses++;
        seen_blocks.insert(unique_id);
    } else {
        stats.cap_misses++; // TODO: optionally split conflict vs capacity with more logic (pdf)
    }

    int evict_index = chooseBlockToEvict(set_index);
    if (set[evict_index].valid) {
        stats.evictions++;
    }

    set[evict_index].valid = true;
    set[evict_index].tag = tag;
    set[evict_index].last_used = global_timestamp;

    return false;
}