#pragma once
#include "core.h"
#include "handle.h"
#include <vector>
#include <expected>
#include <map>
#include <type_traits>
#include "logger.h"

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

    using MeshKey = std::string;                             // filePath
    using TextureKey = std::string;                          // filePath
    using MaterialKey = std::pair<std::string, std::string>; // {filePath, name}

    // =============================== Pool ====================================
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

    // ============================== Register =================================
    enum class OverwritePolicy
    {
        KeepExisted,
        Replace,
        Error
    };

    template <class HandleT, class KeyT> struct RegisterOutcome
    {
        enum class Status
        {
            Reused,
            Inserted,
            Replaced,
            Failed
        } status;
        HandleT                  handle;
        KeyT                     key;
        std::optional<ErrorCode> err;
    };

    template <class HandleT, class KeyT>
    bool isRegisterFailed(const RegisterOutcome<HandleT, KeyT> &res)
    {
        using Status = typename RegisterOutcome<HandleT, KeyT>::Status;
        return (res.status == Status::Failed);
    }

    template <class HandleT, class KeyT>
    void logRegisterOutcome(const RegisterOutcome<HandleT, KeyT> &res, std::string_view id)
    {
        using Status = typename RegisterOutcome<HandleT, KeyT>::Status;
        std::string key;
        if constexpr (std::is_same<KeyT, MaterialKey>::value)
            key = res.key.first + res.key.second;
        else
            key = res.key;

        // fail
        if (res.status == Status::Failed)
        {
            std::string_view reason = res.err ? getErrorMessage(*res.err) : "<unknown>";
            LOG_ERROR("register failed: id='", key, "', reason=", reason);
            return;
        }

        //  success
        switch (res.status)
        {
        case Status::Reused:
            LOG_DEBUG("id='", key, "' reused (handle=", res.handle, ")");
            break;
        case Status::Inserted:
            LOG_DEBUG("id='", key, "' inserted (handle=", res.handle, ")");
            break;
        case Status::Replaced:
            LOG_INFO("id='", key, "' replaced (handle=", res.handle, ")");
            break;
        case Status::Failed: // 위에서 이미 처리
            break;
        }
    }

    // =============================== Manager =================================

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

        RegisterOutcome<MeshHandle, MeshKey>
        registerMesh(const MeshKey &key, const core::Mesh *init = nullptr,
                     OverwritePolicy pol = OverwritePolicy::KeepExisted);

        RegisterOutcome<MaterialHandle, MaterialKey>
        registerMaterial(const MaterialKey &key, const core::Material *init = nullptr,
                         OverwritePolicy pol = OverwritePolicy::KeepExisted);

        RegisterOutcome<TextureHandle, TextureKey>
        registerTexture(const TextureKey &key, const core::Texture *init = nullptr,
                        OverwritePolicy pol = OverwritePolicy::KeepExisted);
    };

} // namespace resource
