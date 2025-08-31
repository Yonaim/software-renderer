#pragma once
#include "handle.h"
#include "math/math.h"
#include <cstdint>
#include <vector>
#include <algorithm>

namespace core
{
#define X(name, msg) name,
    enum class ErrorCode
    {
#include "core_error.def"
#include "common_error.def"
    };
#undef X

#define X(name, msg)                                                                               \
    case ErrorCode::name:                                                                          \
        return msg;
    inline const char *getErrorMessage(ErrorCode e)
    {
        switch (e)
        {
#include "core_error.def"
#include "common_error.def"
        }
        return "Unknown error";
    }
#undef X

    struct Vertex
    {
        math::Vec3 position;
        math::Vec3 normal;
        math::Vec3 tangent;
        math::Vec2 uv;
    };

    struct Submesh
    {
        std::string    id; // MaterialName_#Number
        MaterialHandle material;
        uint32_t       idxStart;
        uint32_t       idxEnd;
    };

    struct Mesh
    {
        std::vector<Submesh>  subs;
        std::vector<Vertex>   vertices;
        std::vector<uint32_t> indices;
        // TODO: AABB 등 최적화용 자료구조 추가
    };

    struct Texture
    {
        int                     width, height = 0;
        std::vector<math::Vec4> data; // RGBA

        math::Vec4 sample(int u, int v) const { return data[width * v + u]; }
        math::Vec4 sample(math::Vec2 uv) const { return data[width * uv.y + uv.x]; }
    };

    // 우선 Emmisive는 자체 발광만 함 (주변 사물에 영향x)
    struct Material // PBR
    {
        // uniform pasrameters
        math::Vec3 albedo;
        float      metallic;
        float      roughness;
        float      emissive;
        float      reflectance; // Fresnel 효과 (TODO: 공부 필요)

        // texture maps
        // TODO: metallic map, emmisive map 추가 고려
        TextureHandle albedoMap;
        TextureHandle normalMap;
    };

    struct FrameBuffer
    {
        int                  width, height;
        std::vector<uint8_t> color; // RGB256

        FrameBuffer(int w, int h) : width(w), height(h), color(w * h * 4, 0) {}

        void clear(const math::Vec4 &rgba)
        {
            auto    clamp_0_1 = [](float n) { return std::max(0.0f, std::min(n, 1.0f)); };
            uint8_t r = clamp_0_1(rgba.x) * 255.0f;
            uint8_t g = clamp_0_1(rgba.y) * 255.0f;
            uint8_t b = clamp_0_1(rgba.z) * 255.0f;
            uint8_t a = clamp_0_1(rgba.w) * 255.0f;

            for (int i = 0; i < width * height; i++)
            {
                color[i * 4 + 0] = r;
                color[i * 4 + 1] = g;
                color[i * 4 + 2] = b;
                color[i * 4 + 3] = a;
            }
        }

        void writeRGBA(int x, int y, const math::Vec4 &rgba)
        {
            if (!(0 <= x && x <= width) || !(0 <= y && y <= height))
                return;
            auto clamp_0_1 = [](float n) { return std::max(0.0f, std::min(n, 1.0f)); };
            int  idx = (y * width + x) * 4;

            color[idx + 0] = clamp_0_1(rgba.x) * 255.0f;
            color[idx + 1] = clamp_0_1(rgba.y) * 255.0f;
            color[idx + 2] = clamp_0_1(rgba.z) * 255.0f;
            color[idx + 3] = clamp_0_1(rgba.w) * 255.0f;
        }
    };

    struct DepthBuffer
    {
        int                width, height;
        std::vector<float> depth; // [0 , 1]

        DepthBuffer(int w, int h) : width(w), height(h), depth(w * h, 1.0f) {}

        void clear(float z) { std::fill(depth.begin(), depth.end(), z); }

        bool testAndWrite(int x, int y, float z)
        {
            if (!(0 <= x && x <= width) || !(0 <= y && y <= height))
                return false;
            int idx = (y * width) + x;
            if (z < depth[idx])
            {
                depth[idx] = z;
                return true;
            }
            else
                return false;
        }
    };
} // namespace core
