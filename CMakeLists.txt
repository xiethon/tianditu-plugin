cmake_minimum_required(VERSION 3.22.1)

project(
    TiandituPlugin
    VERSION 1.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/xiethon/tianditu-plugin"
    DESCRIPTION "A Qt Location Service Provider Plugin for TianDitu Maps"
)

option(BUILD_EXAMPLE "Build example" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif(BUILD_EXAMPLE)

