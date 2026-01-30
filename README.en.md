# Tianditu Location Plugin

[中文](README.md) | [English](README.en.md)

A Qt Location Service Provider Plugin that enables seamless integration of TianDiTu (天地图) and other map services into Qt applications.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

<img src="assets/example.png" width="600" height="400">

## Features

- 🗺️ **TianDiTu Integration** - Full support for TianDiTu online and offline mapping services
- � **Reverse Geocoding** - Support for querying address information based on coordinates
- 💾 **Tile Caching** - SQLite-based tile caching for offline usage
- 🔧 **Flexible Configuration** - JSON-based configuration for easy customization. Configure any map source through resources_url_config.json

## Requirements

- Qt 6.5+
- CMake 3.22+
- C++17 compiler

## Installation

### Method 1: Using CPM (Recommended)

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) is a modern CMake dependency management tool.

Add to your project:

```cmake
# Download CPM.cmake (if not already)
include(cmake/CPM.cmake)

# Add tianditu-plugin
CPMAddPackage(
    NAME tianditu-plugin
    GITHUB_REPOSITORY xiethon/tianditu-plugin
    GIT_TAG main  # or specify a tag
    OPTIONS
        "BUILD_EXAMPLE OFF"  # Don't build example
)

# Link to your target
target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
```

### Method 2: Using FetchContent (CMake built-in)

```cmake
include(FetchContent)

FetchContent_Declare(
    tianditu-plugin
    GIT_REPOSITORY https://github.com/xiethon/tianditu-plugin.git
    GIT_TAG main
)

set(BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)  # Don't build example
FetchContent_MakeAvailable(tianditu-plugin)

target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
```

### Method 3: Using find_package (System Installation)

1. **Install to system**
   ```bash
   cmake -B build -DBUILD_EXAMPLE=OFF
   cmake --build build
   sudo cmake --install build
   ```

2. **Use in your project**
   ```cmake
   find_package(TiandituPlugin 1.0 REQUIRED)
   target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
   ```

### Method 4: As Submodule (Traditional)

1. **Clone the repository to your project directory**
   ```bash
   git clone https://github.com/xiethon/TiandituPlugin.git
   ```

2. **CMake Configuration**
   ```bash
   add_subdirectory(TiandituPlugin)
   target_link_libraries(${YOUR_PROJECT_NAME} PRIVATE TiandituPlugin::tianditu-plugin)
   ```

### Build Example

```bash
cmake -B build -DBUILD_EXAMPLE=ON
cmake --build build -j $(nproc)
   ./build/example/tianditu-plugin-example
   ```

## Usage

Using TianDiTu plugin in QML:

```qml
Map {
    plugin: Plugin {
        name: "Tianditu"
        PluginParameter {
            name: "Tianditu.token"
            value: "Your TianDiTu API Key" 
        }
    }
    // ...
    Component.onCompleted: {
        setActiveMap("Tianditu", MapType.SatelliteMapDay);  // Set to satellite imagery
    }
}
```

For more details, see the example code [Example.qml](example/Example.qml)

### Configuring Other Map Sources

Configure other map sources through **resources_url_config.json**. Example using Amap (高德地图):

```json
{
    "pluginName": "Tianditu",
    "mapSources": [
        {
            "provider": "Amap",
            "copyright": "&copy; <a href='https://lbs.amap.com/'>Amap</a> contributors",
            "styles": [
                {
                    "style": "StreetMap",
                    "url": "https://webst01.is.autonavi.com/appmaptile?style=8&x={x}&y={y}&z={z}"
                },
                {
                    "style": "SatelliteMapDay",
                    "url": "https://webst01.is.autonavi.com/appmaptile?style=6&x={x}&y={y}&z={z}"
                },
                {
                    "style": "TerrainMap",
                    "url": "https://webrd04.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x={x}&y={y}&z={z}"
                },
                {
                    "style": "CustomMap",
                    "url": "https://webst01.is.autonavi.com/appmaptile?style=8&x={x}&y={y}&z={z}"
                }
            ]
        }
    ]
}
```

**Parameters:**
* `provider`: Map provider name, corresponds to the first parameter in `setActiveMap()`
* `style`: Map style, corresponds to the second parameter in `setActiveMap()`

**Usage Example:**

Set Amap satellite imagery: `setActiveMap("Amap", MapType.SatelliteMapDay)`

> **Note**: Some map sources require an API token for access. Please refer to each map provider's service policy.

For more available map sources in China, see [resources_url_config.json](assets/resources_url_config.json)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Support

- 📧 Email: xiethon@163.com, xiethon2020@gmail.com
- 🔗 Repository: https://github.com/xiethon/tianditu-plugin
