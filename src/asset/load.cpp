#include "asset.h"

namespace asset
{
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
        scn.camera.fovY = scn_config.camera.fov;
        scn.camera.znear = scn_config.camera.znear;
        scn.camera.zfar = scn_config.camera.zfar;

        // objects
        for (auto obj : scn.objects)
        {
            // TODO: fileIO 모듈 사용해 resourceManager에 asset 넣기
        }
        return scn;
    }
} // namespace asset