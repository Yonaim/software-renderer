#pragma once
#include "core.h"
#include "fileIO.h"
#include "resource.h"
#include "scene.h"
#include <filesystem>
#include <expected>
#include <string>
#include <variant>
#include <span>

namespace fs = std::filesystem;

/*
[File Format]
- Geometry: obj
- Material: mtl
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

    inline auto floatsToVec3 = [](float *f_s) -> math::Vec3
    { return {*f_s, *(f_s + 1), *(f_s + 2)}; };

    template <class T> using Result = std::expected<T, ErrorCode>;

    namespace parser
    {
        struct CameraConfig
        {
            math::Vec3 pos;
            math::Vec3 rot;
            float      fov;
            float      near;
            float      far;
        };

        using LightConfig = scene::Light;

        struct GeometryConfig
        {
            std::string_view id;
            std::string_view file;
        };

        struct MaterialConfig
        {
            std::string_view id;
            std::string_view file;
            std::string_view name;
        };

        struct ObjectConfig
        {
            std::string_view id;
            std::string_view meshId;
            math::Vec3       pos;
            math::Vec3       rot;
            math::Vec3       scale;
        };

        struct SceneConfig
        {
            std::string                 name;
            CameraConfig                camera;
            std::vector<LightConfig>    lights;
            std::vector<MaterialConfig> materials;
            std::vector<GeometryConfig> geometries;
            std::vector<ObjectConfig>   objects;
        };

        // one instance per one file
        // file명은 caller가 추가해서 전체 Materialkey를 완성해야 함
        struct MaterialEntry
        {
            std::string_view name;
            core::Material   material;
        };
        using MaterialEntries = std::unordered_map<std::string_view, core::Material>;

        enum class PixelFormat
        {
            Gray8,
            GA8,
            RGB8,
            RGBA8,
            RGB16,
            RGBA16
        };

        enum class TransferFunc
        {
            NonLinear,
            Linear
        };

        struct ImageBuffer // sRGB
        {
            int                  width, height;
            PixelFormat          format;
            TransferFunc         transFunc;
            std::vector<uint8_t> pixels;
        };

        // text format
        Result<core::Mesh>      obj(std::string_view text);
        Result<MaterialEntries> mtl(std::string_view text);
        Result<SceneConfig>     json(std::string_view text);
        // binary format
        Result<ImageBuffer> png(std::span<const std::byte> bytes);
        Result<ImageBuffer> ppm(std::span<const std::byte> bytes);
    } // namespace parser

    namespace loader
    {
        using KeyedMaterial = std::pair<resource::MaterialKey, core::Material>;
        using KeyedMaterialList = std::vector<std::pair<resource::MaterialKey, core::Material>>;

        // load whole scene and all resources
        Result<scene::Scene> loadSceneAndResources(const fs::path    &sceneJson,
                                                   resource::Manager &mgr);
        // load & parsing
        Result<parser::SceneConfig> loadSceneConfig(const fs::path &jsonPath);
        Result<core::Mesh>          loadMesh(const fs::path &objPath);
        Result<core::Material>      loadMaterial(const fs::path &mtlPath, std::string_view name);
        Result<KeyedMaterialList>   loadMaterialList(const fs::path &mtlPath);
        Result<parser::ImageBuffer> loadImage(const fs::path &imgPath);

    } // namespace loader
} // namespace asset
