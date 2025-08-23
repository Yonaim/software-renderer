#include "fileIO.h"

namespace fileIO
{
    bool writeBytes(const std::string &src, const std::string &dst)
    {
        return true;
    }

    bool writeImage(const core::FrameBuffer &src, const std::string &dst)
    {
        return true;
    }

    std::vector<std::byte> readBytes(const std::string &src)
    {
        std::vector<std::byte> bytes;
        return bytes;
    }

} // namespace fileIO
