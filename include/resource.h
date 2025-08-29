#pragma once
#include "core.h"
#include "handle.h"
#include <vector>

namespace resource
{
#define X(name, msg) name,
    enum class ErrorCode
    {
#include "common_error.def"
#include "resource_error.def"
    };
#undef X

#define X(name, msg)                                                                               \
    case ErrorCode::name:                                                                          \
        return msg;
    inline const char *getErrorMessage(ErrorCode e)
    {
        switch (e)
        {
#include "common_error.def"
#include "resource_error.def"
        }
        return "Unknown error";
    }
#undef X

    class Manager
    {
      private:
        std::vector<core::Mesh>     meshes;
        std::vector<core::Texture>  textures;
        std::vector<core::Material> materials;

      public:
        core::Mesh    &getMesh(MeshHandle handle);
        core::Texture &getTexture(TextureHandle handle);
        core::Texture &getMaterial(MaterialHandle handle);
        uint32_t       addMesh(core::Mesh &m);
        uint32_t       addTexture(core::Texture &t);
        uint32_t       addMaterial(core::Material &m);
    };

} // namespace resource
