#include "HuffmanArchiver.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace huffman {

constexpr const char* VERSION = "1.0.0";
constexpr const char* FILE_EXTENSION = ".huff";

HuffmanArchiver::HuffmanArchiver()
    : fileCompressor(std::make_unique<FileCompressor>())
{}


std::string HuffmanArchiver::getExtension(const std::string& path) {
    return fs::path(path).extension().string();
}

std::string HuffmanArchiver::removeExtension(const std::string& path) {
    return fs::path(path).replace_extension("").string();
}

bool HuffmanArchiver::compress(const std::vector<std::string>& sources, const std::string& output) {
    try {
        for (const auto& source : sources) {
            if (!fs::exists(source)) {
                std::cerr << "ERORR: 文件不存在: " << source << std::endl;
                return false;
            }
        }

        // 确定输出路径
        std::string actualOutput = output;
        if (actualOutput.empty()) {
            actualOutput = fs::current_path().string() + FILE_EXTENSION;
        }

        // 检查输出文件是否已经存在
        if (fs::exists(actualOutput)) {
            std::cout << "WARNNING: 输出文件已存在: " << actualOutput
                << "\n是否确认覆盖? (y/n): ";
            char confirm;
            std::cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') {
                return false;
            }
        }

        // 打包并压缩数据
        std::vector<uint8_t> packedData = packer->pack(sources);
        fileCompressor->compressToFile(packedData, actualOutput);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "ERORR: " << e.what() << std::endl;
        return false;
    }
}

bool HuffmanArchiver::decompress(const std::string& source,
                                 const std::string& output) {
    try {
        if (!fs::exists(source)) {
            std::cerr << "ERORR: 压缩文件不存在: " << source << std::endl;
            return false;
        }

        // 确定输出路径
        std::string actualOutput = output;
        if (actualOutput.empty()) {
            // 如果源文件有.huff扩展名，移除它
            if (getExtension(source) == FILE_EXTENSION) {
                actualOutput = removeExtension(source);
            } else {
                actualOutput = source + "_extracted";
            }
        }

        // 检查输出路径是否已存在
        if (fs::exists(actualOutput)) {
            std::cout << "WARNNING: 输出路径已存在: " << actualOutput
                << "\n是否确认覆盖? (y/n): ";
            char confirm;
            std::cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') {
                return false;
            }
        }

        // 从文件解压数据
        std::vector<uint8_t> packedData;
        fileCompressor->decompressFromFile(source, packedData);

        // 解包数据
        packer->unpack(packedData, actualOutput);
        
        return true;

    } catch (const std::exception& e) {
        std::cerr << "ERORR: " << e.what() << std::endl;
        return false;
    }
}

std::string HuffmanArchiver::getVersion() {
    return VERSION;
}

void HuffmanArchiver::printInfo() {
    std::string info = "哈夫曼编码解压缩工具 v" + getVersion();
    std::cout << info << std::endl;
}

}