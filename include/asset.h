#pragma once
#include "core.h"
#include "fileIO.h"
#include "resource.h"
#include "scene.h"
#include <exception>
#include <string>
#include <variant>

/*
[File Format]
- Mesh: obj, mtl
- Texture: png, ppm
- Scene: json
*/
namespace asset
{
#define X(name, msg) name,
    enum class ErrorCode
    {
#include "common_error.def"
#include "asset_error.def"
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
#include "asset_error.def"
        }
        return "Unknown error";
    }
#undef X

    using AssetData =
        std::variant<std::monostate, core::Mesh, core::Texture, core::Material, scene::Scene>;

    inline auto floatsToVec3 = [](float *f_s) -> math::Vec3
    { return {*f_s, *(f_s + 1), *(f_s + 2)}; };

    struct Entry
    {
        std::string ext;
        parseFunc   f;
    };

    // TODO: simdjson JSON 파서 사용
    namespace parser
    {
        using Texture = std::pair<std::string_view, std::string_view>;

        struct sceneCameraConfig
        {
            float pos[3];
            float rot[3];
            float fov;
            float zfar;
            float znear;
        };

        struct sceneObjectConfig
        {
            std::string_view     mesh;
            std::string_view     material;
            std::vector<Texture> textures;
            float                pos[3] = {0, 0, 0};
            float                rot[3] = {0, 0, 0};
            float                scale = 1;
        };

        struct sceneConfig
        {
            sceneCameraConfig              camera;
            std::vector<sceneObjectConfig> objects;
        };

        // TODO: 파싱 코드 작성
        AssetData obj(const std::string &raw);
        AssetData mtl(const std::string &raw);
        AssetData png(const std::string &raw);
        AssetData ppm(const std::string &raw);
        AssetData json(const std::string &raw);
    } // namespace parser

    const Entry entries[] = {{"obj", parser::obj},
                             {"mtl", parser::mtl},
                             {"png", parser::png},
                             {"ppm", parser::ppm},
                             {"json", parser::json}};

    inline parseFunc findParser(std::string_view path)
    {
        size_t pos = path.rfind(".");
        if (pos != std::string_view::npos)
        {
            const std::string_view ext = path.substr(pos + 1);
            for (auto e : entries)
                if (e.ext == ext)
                    return e.f;
        }
        return nullptr;
    }

    AssetData    loadAsset(const std::string &path);
    scene::Scene loadScene(const std::string &path, resource::Manager &resourceManager);
} // namespace asset
