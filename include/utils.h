#pragma once
#include <optional>
#include "Cache.h"
#include <string>

namespace Utils {

    /**
     * @brief Parses tokens and binds them to their specific cache configuration.
     *
     * Checks if the string is properly formatted and if there is an input file specified.
     * If not, skips the config and returns a failed state.
     *
     * @param configStr String that contains the raw config.
     * @return On success, returns the modified cache config struct.
     *         
     * On failure, returns a `std::nullopt`.
     */
    std::optional<Cache::Config> parseCacheConfig(const std::string& configStr);

    /**
     * @brief Reads 32-bit addresses from a binary file.
     *
     * Opens the given file in binary mode and reads 4 bytes at a time, interpreting
     * each as a `uint32_t` address. Returns the parsed addresses if successful.
     *
     * @param filename Path to the input file containing binary address data.
     * @return On success, returns a vector of 32-bit addresses.
     *         
     * On failure, returns `std::nullopt`.
     */
    std::optional<std::vector<uint32_t>> readBinaryAddresses(const std::string& filename);
}