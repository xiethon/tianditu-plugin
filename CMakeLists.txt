cmake_minimum_required(VERSION 3.22.1)

project(
    tianditu-plugin
    VERSION 1.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/xiethon/tianditu-plugin"
    DESCRIPTION "A Qt Location Service Provider Plugin for TianDitu Maps"
)

option(BUILD_EXAMPLE "Build example" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(src)

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif(BUILD_EXAMPLE)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT TiandituPluginTargets
    FILE TiandituPluginTargets.cmake
    NAMESPACE TiandituPlugin::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TiandituPluginConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TiandituPluginConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TiandituPlugin
)

