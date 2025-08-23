#pragma once
#include "core.h"
#include <cstddef>
#include <string>
#include <vector>

namespace fileIO
{
    bool writeBytes(const std::string &src, const std::string &dst);
    bool writeImage(const core::FrameBuffer &src, const std::string &dst);
    std::vector<std::byte> readBytes(const std::string &src);
} // namespace fileIO
