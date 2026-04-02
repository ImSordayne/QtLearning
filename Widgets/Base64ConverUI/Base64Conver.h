/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \Base64ConverUI\src\Base64Conver.h
 */
#include <filesystem>
#include <functional>
namespace fs = std::filesystem;

namespace Base64Conver {
    bool encFile(fs::path inputFile, fs::path outputFile, std::function<void*(size_t, size_t)> callback = nullptr);
    bool decFile(fs::path inputFile, fs::path outputFile, std::function<void*(size_t, size_t)> callback = nullptr);

    std::string decode(const std::string& data);
    std::string encode(const unsigned char* data, size_t len);
}