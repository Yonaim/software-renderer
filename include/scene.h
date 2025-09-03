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
            : pos(p), rot(r), fovY(fovy), znear(zn), zfar(zf)
        {
        }
    };

    enum class LightType
    {
        Directional,
        Point,
        Spot,
        Ambient
    };

    struct Light
    {
        std::string id;
        LightType   type; // directional, point, spot, ambient
        math::Vec3  color{1, 1, 1};
        float       intensity{1.0f};
        bool        castShadow{false};

        union
        {
            struct
            {
                math::Vec3 dir;
            } directional;
            struct
            {
                math::Vec3 pos;
                float      range;
            } point;
            struct
            {
                math::Vec3 pos;
                math::Vec3 dir;
            } spot;
            struct
            { /* ambient has nothing extra */
            } ambient;
        };

        Light() {}
    };

    struct HandlesById // id -> handle
    {
        std::unordered_map<std::string_view, MeshHandle>     mesh;
        std::unordered_map<std::string_view, MaterialHandle> material;
        std::unordered_map<std::string_view, TextureHandle>  texture;
    };

    struct Scene
    {
        std::string         name;
        Camera              camera;
        std::vector<Light>  lights;
        std::vector<Object> objects;
        HandlesById         handlesById;

        Scene() = default;
        void addObject(const Object &obj);
        void addLight(const Light &light);
        void setCamera(const Camera &cam);
    };

} // namespace scene
