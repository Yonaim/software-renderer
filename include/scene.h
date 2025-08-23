#pragma once
#include "handle.h"
#include "math/vec.h"
#include <vector>

namespace scene
{

    struct Object
    {
        MeshHandle     mesh;
        MaterialHandle mat;
        TextureHandle  tex;
        math::Vec3     pos;
        math::Vec3     rot;
        math::Vec3     scale;
    };

    struct Camera
    {
        math::Vec3 pos;
        math::Vec3 rot;
        float      fov; // fovY
        float      near;
        float      far;
    };

    struct Scene
    {
        std::vector<Object> objects;
        Camera              camera;
        void                addObject(const Object &obj);
        void                setCamera(const Camera &cam);
    };

} // namespace scene
