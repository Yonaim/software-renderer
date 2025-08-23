#pragma once
#include "core.h"
#include "scene.h"
#include <cstdint>

namespace window
{
    struct Manager
    {
        uint32_t id;
    };

    WindowHandle createInstance();
    void         showBuffer(const core::FrameBuffer &src);

} // namespace window
