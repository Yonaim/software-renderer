#pragma once
#include <string>
#include "core.h"

namespace fileIO
{
    int writeBytes(const std::string &src, const std::string &dst);
    int writeImage(const core::FrameBuffer &src, const std::string &dst);
    std::string readBytes(const std::string &src);
} // namespace fileIO

