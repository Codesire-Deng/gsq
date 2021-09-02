#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace FileBuffer {

inline std::string binaryFromFile(std::string_view path) {
    size_t size = std::filesystem::file_size(path);
    std::string result(size, '\0');
    std::ifstream fin(path, std::ios::in | std::ios::binary);
    fin.read(result.data(), size);
    return result;
}

} // namespace FileBuffer
