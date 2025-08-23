#include "renderer.h"

namespace renderer
{
    int Renderer::render(const scene::Scene &scn, core::FrameBuffer &fb,
                         core::DepthBuffer &db)
    {
        return 0;
    }

    void Renderer::setVertexShader(const shader::VS &vs) {}

    void Renderer::setFragmentShader(const shader::FS &vs) {}

}; // namespace renderer
