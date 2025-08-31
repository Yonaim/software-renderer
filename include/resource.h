#pragma once
#include "core.h"
#include "handle.h"
#include <vector>
#include <expected>
#include <map>

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
    template <class T> using Result = std::expected<T, ErrorCode>;

    enum class OverwritePolicy
    {
        Error,
        KeepExisted,
        Replace
    };

    using MeshKey = std::string;
    using TextureKey = std::string;
    using MaterialKey = std::pair<std::string, std::string>; // {file, name}

    struct MeshPool
    {
        std::vector<core::Mesh>           items;
        std::map<std::string, MeshHandle> idToHandle;
    };

    struct MaterialPool
    {
        std::vector<core::Material>           items;
        std::map<MaterialKey, MaterialHandle> idToHandle;
    };

    struct TexturePool
    {
        std::vector<core::Texture>           items;
        std::map<std::string, TextureHandle> idToHandle;
    };

    enum class OverwritePolicy
    {
        KeepExisted,
        Replace,
        Error
    };

    enum class RegisterOutcome
    {
        Reused,
        Inserted,
        Replaced
    };

    template <class HandleT>
    using RegisterResult = std::expected<std::pair<HandleT, RegisterOutcome>, ErrorCode>;

    class Manager
    {
      private:
        MeshPool     meshes;
        MaterialPool materials;
        TexturePool  textures;

      public:
        // get
        core::Mesh     &getMesh(MeshHandle handle) const;
        core::Material &getMaterial(MaterialHandle handle) const;
        core::Texture  &getTexture(TextureHandle handle) const;

        RegisterResult<MeshHandle> registerMesh(const MeshKey    &key,
                                                const core::Mesh *init = nullptr,
                                                OverwritePolicy pol = OverwritePolicy::KeepExisted);

        RegisterResult<MaterialHandle>
        registerMaterial(const MaterialKey &key, const core::Material *init = nullptr,
                         OverwritePolicy pol = OverwritePolicy::KeepExisted);

        RegisterResult<TextureHandle>
        registerTexture(const TextureKey &key, const core::Texture *init = nullptr,
                        OverwritePolicy pol = OverwritePolicy::KeepExisted);
    };

} // namespace resource
