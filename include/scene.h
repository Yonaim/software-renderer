#pragma once
#include "handle.h"
#include "math/vec.h"
#include <optional>
#include <variant>
#include <vector>

// TODO: 기본 값 설정
namespace scene
{
    struct Object
    {
        MeshHandle                   mesh;
        MaterialHandle               mat;
        std::optional<TextureHandle> tex;
        math::Vec3                   pos;
        math::Vec3                   rot;
        math::Vec3                   scale;
    };

    struct Camera
    {
        math::Vec3 pos;
        math::Vec3 rot;
        float      fovY;
        float      znear;
        float      zfar;
    };

    namespace light
    {
        struct Common
        {
            math::Vec3 color;
            float      intensity;
        };

        struct Directional : Common
        {
            math::Vec3 dir;
            bool       castShadow;
        };

        struct Point : Common
        {
            math::Vec3 pos;
            float      range;
            bool       castShadow;
        };

        struct Spot : Common
        {
            math::Vec3 pos;
            math::Vec3 dir;
            bool       castShadow;
        };

        struct Ambient : Common
        {
        };
    } // namespace light

    using Light = std::variant<light::Directional, light::Point, light::Spot,
                               light::Ambient>;

    struct Scene
    {
        std::vector<Object> objects;
        std::vector<Light>  lights;
        Camera              camera;
        void                addObject(const Object &obj);
        void                addLight(const Light &light);
        void                setCamera(const Camera &cam);
    };

} // namespace scene
