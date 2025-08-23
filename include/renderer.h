#pragma once
#include "core.h"
#include "math/math.h"
#include "scene.h"
#include <functional>

namespace renderer
{
    // 차후 쉐이더 프로그램 추가
    namespace shader
    {
        struct VSUniform
        {
            math::Mat4 M, V, P;
            math::Mat4 N; // normal matrix (normai is covector)
        };

        struct FSUniform
        {
            std::vector<scene::Light> lights;
            float                     gamma = 2.2f;
        };

        struct VSIn
        {
            math::Vec3 local_pos;
            math::Vec3 local_nrm;
            math::Vec4 color = {1, 1, 1, 1};
        };

        struct VSOut
        {
            math::Vec4 clip_pos;
            math::Vec3 world_pos;
            math::Vec3 world_nrm;
            math::Vec3 color;
        };

        struct FSIn
        {
            math::Vec3 nrm;
            math::Vec3 color;
        };

        struct FSOut
        {
            float      depth;
            math::Vec4 color;
        };

        using VS = std::function<VSOut(const VSIn &)>;
        using FS = std::function<FSOut(const FSIn &)>;

        VSOut defaultVS(const VSIn &);
        FSOut defaultFS(const FSIn &);
    } // namespace shader

    class Renderer
    {
      private:
        shader::VS vs = shader::defaultVS;
        shader::FS fs = shader::defaultFS;

      public:
        int  flags;
        int  render(const scene::Scene &scn, core::FrameBuffer &fb,
                    core::DepthBuffer &db);
        void setVertexShader(const shader::VS &vs);
        void setFragmentShader(const shader::FS &vs);
    };
} // namespace renderer
