cmake_minimum_required(VERSION 3.31)
project(CacheSimAS)

set(CMAKE_CXX_STANDARD 20)

add_executable(CacheSimAS main.cpp
        Cache.cpp
        Cache.h
        CacheSimulator.cpp
        CacheSimulator.h)
