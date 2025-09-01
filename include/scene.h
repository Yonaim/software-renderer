#pragma once
#include "handle.h"
#include "math/vec.h"
#include <optional>
#include <variant>
#include <vector>
#include <unordered_map>

// TODO: 기본 값 설정
namespace scene
{
    struct Object
    {
        std::string id;
        MeshHandle  mesh;
        math::Vec3  pos;
        math::Vec3  rot;
        math::Vec3  scale;
    };

    struct Camera
    {
        math::Vec3 pos;
        math::Vec3 rot;
        float      fovY;
        float      znear;
        float      zfar;

        Camera() = default;
        Camera(const math::Vec3 &p, const math::Vec3 &r, float fovy, float zn, float zf) noexcept
            : pos(p), rot(r), fovY(fovy), znear(zn), zfar(zf);
    };

    namespace light
    {
        struct Common
        {
            std::string id;
            math::Vec3  color;
            float       intensity;
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

    using Light = std::variant<light::Directional, light::Point, light::Spot, light::Ambient>;

    struct Scene
    {
        std::string         name;
        Camera              camera;
        std::vector<Light>  lights;
        std::vector<Object> objects;

        struct HandlesById // id -> handle
        {
            std::unordered_map<std::string, MeshHandle>     mesh;
            std::unordered_map<std::string, MaterialHandle> material;
            std::unordered_map<std::string, TextureHandle>  texture;
        };

        Scene() = default;
        void addObject(const Object &obj);
        void addLight(const Light &light);
        void setCamera(const Camera &cam);
    };

} // namespace scene
