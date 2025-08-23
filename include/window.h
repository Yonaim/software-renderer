#pragma once
#include <cstdint>
#include "scene.h"
#include "core.h"

namespace window
{

    struct Manager
    {
        uint32_t id;
    };

    WindowHandle createInstance();
    void         showBuffer(const core::FrameBuffer &src);

} // namespace window
