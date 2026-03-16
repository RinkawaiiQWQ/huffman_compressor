cmake_minimum_required(VERSION 3.10)
project(huffman_compressor CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 设置头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_SOURCE_DIR}/include/thirdparty)

# 定义源文件列表（排除 DirectoryCompressor 相关文件）
set(SOURCES
    src/BitStream.cpp
    src/FileCompressor.cpp
    src/Packer.cpp
    src/HuffmanArchiver.cpp
    src/HuffmanTree.cpp
    src/main.cpp
)

# 添加可执行文件
add_executable(huffman_compressor ${SOURCES})