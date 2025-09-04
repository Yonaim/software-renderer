#pragma once
#include "core.h"
#include <cstddef>
#include <string>
#include <vector>

namespace fileIO
{
#define X(name, msg) name,
    enum class ErrorCode
    {
#include "common_error.def"
#include "fileio_error.def"
    };
#undef X
    const char *getErrorMessage(ErrorCode e);

#define X(name, msg)                                                                               \
    case ErrorCode::name:                                                                          \
        return msg;
    inline const char *getErrorMessage(ErrorCode e)
    {
        switch (e)
        {
#include "common_error.def"
#include "fileio_error.def"
        }
        return "Unknown error";
    }
#undef X

    // todo: std::filesystem::path로 바꾸는 것 고려
    std::vector<std::byte> readBytes(const std::string &path);
    std::string            readText(const std::string &path);
    bool                   writeBytes(const std::string &path, const std::vector<uint8_t> &bytes);
    bool writePPM(const std::string &path, int width, int height, const std::vector<uint8_t> &color,
                  bool isRGBA = true);
    bool writePNG(const std::string &path, int width, int height, const std::vector<uint8_t> &color,
                  bool isRGBA = true);
} // namespace fileIO
