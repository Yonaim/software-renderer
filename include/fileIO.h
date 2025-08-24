#pragma once
#include "core.h"
#include <cstddef>
#include <string>
#include <vector>

namespace fileIO
{
    std::vector<std::byte> readBytes(const std::string &path);
    bool writeBytes(const std::string &path, const std::vector<uint8_t> &bytes);
    bool writePPM(const std::string &path, int width, int height,
                  const std::vector<uint8_t> &color, bool isRGBA = true);
    bool writePNG(const std::string &path, int width, int height,
                  const std::vector<uint8_t> &color, bool isRGBA = true);
} // namespace fileIO
