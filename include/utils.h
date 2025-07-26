#pragma once
#include <optional>
#include "Cache.h"
#include <string>

namespace Utils {
    std::optional<Cache::Config> parseCacheConfig(const std::string& configStr);
}