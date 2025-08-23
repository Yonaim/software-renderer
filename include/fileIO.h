#pragma once
#include <string>
#include "core.h"

// TODO: std::vector<std::byte>로 바꾸는 것 고려
namespace fileIO
{
    int writeBytes(const std::string &src, const std::string &dst);
    int writeImage(const core::FrameBuffer &src, const std::string &dst);
    std::string readBytes(const std::string &src);
} // namespace fileIO
