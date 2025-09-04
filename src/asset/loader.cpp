#include "asset.h"
#include "fileIO.h"
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

    Result<parser::SceneConfig> loader::loadSceneConfig(const fs::path &jsonPath)
    {
        std::string text = fileIO::readText(jsonPath);
        return (parser::json(text));
    }

    Result<core::Mesh> loader::loadMesh(const fs::path &objPath)
    {
        std::string text = fileIO::readText(objPath);
        return (parser::obj(text));
    }

    Result<core::Material> loader::loadMaterial(const fs::path &mtlPath, std::string_view name)
    {
        std::string text = fileIO::readText(mtlPath);
        auto        result = parser::mtl(text);
        if (!result)
            return std::unexpected(result.error());

        parser::MaterialEntries &materials = result.value();
        return materials[name];
    }

    Result<parser::MaterialEntries> loader::loadMaterialList(const fs::path &mtlPath)
    {
        std::string text = fileIO::readText(mtlPath);
        return (parser::mtl(text));
    }

    Result<parser::ImageBuffer> loader::loadImage(const fs::path &imgPath)
    {
        std::vector<std::byte> bytes = fileIO::readBytes(imgPath);

        if (imgPath.extension() == "ppm")
            return parser::ppm(bytes);
        else if (imgPath.extension() == "png")
            return parser::png(bytes);
        else
            return std::unexpected(ErrorCode::InvalidParam);
    }
} // namespace asset