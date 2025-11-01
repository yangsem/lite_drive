cmake_minimum_required(VERSION 3.12)
project(lite_drive LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 生成 compile_commands.json 用于IDE支持（如Clangd、VSCode等）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置构建类型（如果没有指定，默认为Debug）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译选项
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/sources/include
    ${CMAKE_SOURCE_DIR}/deps/jsoncpp/include/json
    ${CMAKE_SOURCE_DIR}/deps/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/deps/googletest/googlemock/include
)

# 源文件目录
set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/sources)
set(MODULES_DIR ${SOURCES_DIR}/modules)

# ==================== 收集源文件 ====================

# 工具模块源文件
file(GLOB_RECURSE UTILITIES_SOURCES
    "${MODULES_DIR}/utilities/*.cpp"
    "${MODULES_DIR}/utilities/*.cc"
)

# 网络引擎模块源文件
file(GLOB_RECURSE NET_ENGINE_SOURCES
    "${MODULES_DIR}/net_engine/*.cpp"
    "${MODULES_DIR}/net_engine/*.cc"
)

# 存储模块源文件
file(GLOB_RECURSE STORAGE_SOURCES
    "${MODULES_DIR}/storage/*.cpp"
    "${MODULES_DIR}/storage/*.cc"
)

# 用户管理模块源文件
file(GLOB_RECURSE USER_MANAGER_SOURCES
    "${MODULES_DIR}/user_manger/*.cpp"
    "${MODULES_DIR}/user_manger/*.cc"
)

# 所有共享源文件
set(COMMON_SOURCES
    ${UTILITIES_SOURCES}
    ${NET_ENGINE_SOURCES}
    ${STORAGE_SOURCES}
    ${USER_MANAGER_SOURCES}
)

# ==================== JSONCPP 库 ====================

# 确保jsoncpp构建静态库
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build jsoncpp static library")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build jsoncpp shared library")

# 添加jsoncpp子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/jsoncpp)

# 查找pthread（需要在链接前查找）
find_package(Threads REQUIRED)

# ==================== GoogleTest (可选，用于测试) ====================

# 如果需要测试，可以取消注释以下行
# enable_testing()
# add_subdirectory(${CMAKE_SOURCE_DIR}/deps/googletest)

# ==================== 共享库或静态库（可选） ====================

# 如果需要将公共代码编译为库，可以取消注释以下行
# add_library(lite_drive_common STATIC ${COMMON_SOURCES})
# target_link_libraries(lite_drive_common PUBLIC jsoncpp_static)

# ==================== Server 可执行文件 ====================

# Server源文件
file(GLOB_RECURSE SERVER_SOURCES
    "${SOURCES_DIR}/server/*.cpp"
    "${SOURCES_DIR}/server/*.cc"
)

# 如果server目录下没有源文件，创建一个占位符main
if(NOT SERVER_SOURCES)
    message(STATUS "Warning: No server source files found, you may need to create them")
endif()

add_executable(lite_drive_server
    ${SERVER_SOURCES}
    ${COMMON_SOURCES}
)

target_link_libraries(lite_drive_server PRIVATE
    jsoncpp_static
    Threads::Threads
)

# 设置可执行文件输出路径为项目根目录的bin
set_target_properties(lite_drive_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# ==================== Client 可执行文件 ====================

# Client源文件（命令行版本）
file(GLOB_RECURSE CLIENT_SOURCES
    "${SOURCES_DIR}/client/cmd/*.cpp"
    "${SOURCES_DIR}/client/cmd/*.cc"
)

# 如果client目录下没有源文件，创建一个占位符main
if(NOT CLIENT_SOURCES)
    message(STATUS "Warning: No client source files found, you may need to create them")
endif()

add_executable(lite_drive_client
    ${CLIENT_SOURCES}
    ${COMMON_SOURCES}
)

target_link_libraries(lite_drive_client PRIVATE
    jsoncpp_static
    Threads::Threads
)

# 设置可执行文件输出路径为项目根目录的bin
set_target_properties(lite_drive_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# ==================== 安装规则（可选） ====================

# install(TARGETS lite_drive_server lite_drive_client
#     RUNTIME DESTINATION bin
# )

# ==================== 打印信息 ====================

message(STATUS "==========================================")
message(STATUS "Lite Drive Build Configuration")
message(STATUS "==========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Binary output: ${CMAKE_SOURCE_DIR}/bin")
message(STATUS "Server sources: ${SERVER_SOURCES}")
message(STATUS "Client sources: ${CLIENT_SOURCES}")
message(STATUS "Common sources found: ${COMMON_SOURCES}")
message(STATUS "==========================================")

