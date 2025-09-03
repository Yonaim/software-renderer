#include "asset.h"
#include <variant>

namespace fs = std::filesystem;

namespace asset
{
    // todo: 데이터 유효성 확인
    // todo: 함수 분리
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
        // todo: texture ?
        for (const auto &materialCfg : config.materials)
        {
            // load
            auto materialResult = loadMaterial(materialCfg.file, materialCfg.name);
            if (!materialResult)
                return std::unexpected(materialResult.error());
            resource::MaterialKey key{materialCfg.name};
            core::Material       &material = materialResult.value();

            // register
            auto registerResult = mgr.registerMaterial(key, &material);
            resource::logRegisterOutcome(registerResult, materialCfg.id);
            if (resource::isRegisterFailed(registerResult))
                return std::unexpected(asset::ErrorCode::OperationFail); // todo: errorCode
        }

        // geometries
        for (const auto &geometryCfg : config.geometries)
        {
            // load
            auto meshResult = loadMesh(geometryCfg.file);
            if (!meshResult)
                return std::unexpected(meshResult.error());
            resource::MeshKey key{geometryCfg.file};
            core::Mesh       &mesh = meshResult.value();

            // register
            auto registerResult = mgr.registerMesh(key, &mesh);
            resource::logRegisterOutcome(registerResult, geometryCfg.id);
            if (resource::isRegisterFailed(registerResult))
                return std::unexpected(asset::ErrorCode::OperationFail); // todo: errorCode
        }

        // objects
        for (const auto &objectCfg : config.objects)
        {
            scene::Object obj;
            MeshHandle    meshHandle = outScene.handlesById.mesh[objectCfg.meshId];
            outScene.addObject({std::string(objectCfg.id), meshHandle, objectCfg.pos, objectCfg.rot,
                                objectCfg.scale});
        }

        return outScene;
    }

} // namespace asset