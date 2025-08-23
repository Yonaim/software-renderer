#pragma once
#include <functional>
#include "scene.h"
#include "core.h"

namespace renderer
{
    // 차후 쉐이더 프로그램 추가
    // using VS = std::function<VSOut(const VSIn&)>
    // using FS = std::function<FSOut(const FSIn&)>
    class Renderer
    {
      private:
        // VS vs;
        // FS fs;
      public:
        int flags;
        int render(const scene::Scene &scn, core::FrameBuffer &fb,
                   core::DepthBuffer &db);
    };
} // namespace renderer

