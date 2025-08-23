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
    using AssetData =
        std::variant<core::Mesh, core::Texture, core::Material, scene::Scene>;
    typedef AssetData (*parseFunc)(const std::string &);

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
            float far;
            float near;
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

    parseFunc findParser(std::string_view path)
    {
        const std::string_view ext = path.substr(path.rfind("."));
        for (auto e : entries)
            if (e.ext == ext)
                return e.f;
        return nullptr;
    }

    AssetData loadAsset(const std::string &path)
    {
        const std::string_view path_view(path);
        const std::string_view ext = path_view.substr(path.rfind("."));
        AssetData              loaded;

        try
        {
            loaded = findParser(ext)(path);
        }
        catch (const std::exception &e)
        {
            // error
        }
        return loaded;
    }

    scene::Scene loadScene(const std::string &path,
                           resource::Manager &resourceManager)
    {
        if (findParser(path) != parser::json)
            throw std::runtime_error("e");

        scene::Scene        scn;
        parser::sceneConfig scn_config;

        // camera
        scn.camera.pos = floatsToVec3(scn_config.camera.pos);
        scn.camera.rot = floatsToVec3(scn_config.camera.rot);
        scn.camera.fov = scn_config.camera.fov;
        scn.camera.near = scn_config.camera.near;
        scn.camera.far = scn_config.camera.far;

        // objects
        for (auto obj : scn.objects)
        {
            // TODO: fileIO 모듈 사용해 resourceManager에 asset 넣기
        }
        return scn;
    }

} // namespace asset
