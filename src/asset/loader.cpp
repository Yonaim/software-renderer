#include "asset.h"
#include <variant>

namespace fs = std::filesystem;

namespace asset
{
    // todo: 데이터 유효성 확인
    Result<scene::Scene> loader::loadSceneAndResources(const fs::path    &sceneJson,
                                                       resource::Manager &mgr)
    {
        auto configResult = loadSceneConfig(sceneJson);
        if (!configResult)
            return std::unexpected(configResult.error());

        parser::SceneConfig &config = configResult.value();
        scene::Scene         outScene;
        outScene.name = config.name;

        // camera
        parser::CameraConfig &cam = config.camera;
        outScene.setCamera({cam.pos, cam.rot, cam.fov, cam.near, cam.far});

        // lights
        for (const auto &lightCfg : config.lights)
        {
            outScene.addLight(lightCfg);
        }

        // materials
        for (const auto &materialCfg : config.materials)
        {
            auto materialResult = loadMaterial(materialCfg.file, materialCfg.name);
            if (!materialResult)
                return std::unexpected(materialResult.error());

            resource::MaterialKey key{materialCfg.file, materialCfg.name};
            core::Material       &material = materialResult.value();
            mgr.registerMaterial(key, &material);
        }

        // geometries
        for (const auto &geometryCfg : config.geometries)
        {
            auto geometryResult = loadMesh(geometryCfg.file);
            if (!geometryResult)
                return std::unexpected(geometryResult.error());

            std::string key(geometryCfg.file);
            core::Mesh &mesh = geometryResult.value();
            mgr.registerMesh(key, &mesh);
        }

        // objects
        for (const auto &objectCfg : config.objects)
        {
            // todo: json 객체 내에서 일치하는 id를 찾고 해당 리소스의 핸들을 얻는다
        }

        return outScene;
    }

} // namespace asset