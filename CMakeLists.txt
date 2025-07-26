cmake_minimum_required(VERSION 3.31)
project(CacheSimAS)

set(CMAKE_CXX_STANDARD 20)

add_executable(CacheSimAS main.cpp Cache.cpp CacheSimulator.cpp utils.cpp)

if(WIN32)
    message(STATUS "Configuring for Windows")

    # Include SDL2 headers and link it to .dll.a
    target_include_directories(CacheSimAS PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(CacheSimAS PRIVATE ${CMAKE_SOURCE_DIR}/lib/libSDL2.dll.a)

    # Copy the SDL2.dll next to the executable after build
    add_custom_command(TARGET CacheSimAS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/libSDL2.dll.a
                $<TARGET_FILE_DIR:CacheSimAS>)

elseif(UNIX)
    message(STATUS "Configuring for Linux")

    # Find SDL2 using pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)

    target_include_directories(CacheSimAS PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CacheSimAS PRIVATE ${SDL2_LIBRARIES})
endif()