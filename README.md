# Tianditu Location 插件

[中文](README.md) | [English](README.en.md)

一个为Qt应用程序提供天地图服务集成的Qt Location服务提供商插件，可以将天地图地图服务集成到Qt应用中。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

<img src="assets/example.png" width="600" height="400">

## 功能特性

- 🗺️ **天地图集成** - 完整支持天地图在线和离线地图服务
- 💾 **瓦片缓存** - 基于SQLite的瓦片缓存，支持离线使用
- 🔧 **灵活配置** - 基于JSON的配置，易于自定义地图样式。通过配置resources_url_config.json可以配置任意地图源。

## 系统要求
- Qt 6.5+
- CMake 3.22+
- C++17 编译器

## 安装

### 方法1: 使用 CPM (推荐)

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) 是一个现代的CMake依赖管理工具。

在你的项目中添加：

```cmake
# 下载CPM.cmake（如果还没有）
include(cmake/CPM.cmake)

# 添加 tianditu-plugin
CPMAddPackage(
    NAME tianditu-plugin
    GITHUB_REPOSITORY xiethon/tianditu-plugin
    GIT_TAG v1.0.0  # 或使用 main 分支
    OPTIONS
        "BUILD_EXAMPLE OFF"  # 不构建示例程序
)

# 链接到你的目标
target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
```

### 方法2: 使用 FetchContent (CMake内置)

```cmake
include(FetchContent)

FetchContent_Declare(
    tianditu-plugin
    GIT_REPOSITORY https://github.com/xiethon/tianditu-plugin.git
    GIT_TAG v1.0.0
)

set(BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)  # 不构建示例
FetchContent_MakeAvailable(tianditu-plugin)

target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
```

### 方法3: 使用 find_package (系统安装)

1. **安装到系统**
   ```bash
   cmake -B build -DBUILD_EXAMPLE=OFF
   cmake --build build
   sudo cmake --install build
   ```

2. **在项目中使用**
   ```cmake
   find_package(TiandituPlugin 1.0 REQUIRED)
   target_link_libraries(your_target PRIVATE TiandituPlugin::tianditu-plugin)
   ```

### 方法4: 作为子模块（传统方式）

1. **克隆仓库到项目源码目录**
   ```bash
   git clone https://github.com/xiethon/TiandituPlugin.git
   ```

2. **CMake配置**
   ```bash
   add_subdirectory(TiandituPlugin)
   target_link_libraries(${YOUR_PROJECT_NAME} PRIVATE TiandituPlugin::tianditu-plugin)
   ```

### 构建示例程序

```bash
cmake -B build -DBUILD_EXAMPLE=ON
cmake --build build -j $(nproc)
   ./build/example/tianditu-plugin-example
   ```

## 使用方法

在QML中使用天地图插件：

```qml
Map {
    plugin: Plugin {
        name: "Tianditu"
        PluginParameter {
            name: "Tianditu.token"
            value: "天地图密钥" 
        }
    }
    // ...
    Component.onCompleted: {
        setActiveMap("Tianditu", MapType.SatelliteMapDay);  // 设置为卫星影像图
    }
}
```

详情参见示例代码 [Example.qml](example/Example.qml)

### 配置其他瓦片资源地址教程

通过 **resources_url_config.json** 配置其他地图源,以高德为例：

```json
{
    "pluginName": "Tianditu",
    "mapSources": [
        {
            "provider": "Amap",
            "copyright": "&copy; <a href='https://lbs.amap.com/'>高德地图</a> contributors",
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
**参数说明：**
* `provider`: 地图提供商名称，对应 `setActiveMap()` 的第一个参数
* `style`: 地图样式，对应 `setActiveMap()` 的第二个参数

**使用示例：**

设置高德卫星影像地图：`setActiveMap("Amap", MapType.SatelliteMapDay)`

> **注意**：某些地图源需要 token 才能正常访问，详情请参考各地图提供商的服务政策。

更多国内可用地图源配置请参见 [resources_url_config.json](assets/resources_url_config.json) 


## 许可证

本项目采用MIT许可证 - 详见 [LICENSE.md](LICENSE.md) 文件


## 支持

- 📧 邮箱: xiethon@163.com, xiethon2020@gmail.com
- 🔗 项目地址: https://github.com/xiethon/TiandituPlugin
