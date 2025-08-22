#include "./math/vec.h"
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

using namespace math;

template <class Tag> struct Handle
{
    uint32_t id;
};

struct MeshTag;
struct TextureTag;
struct MaterialTag;
struct ObjectTag;
struct WindowTag;

using MeshHandle = Handle<MeshTag>;
using TextureHandle = Handle<TextureTag>;
using MaterialHandle = Handle<MaterialTag>;
using WindowHandle = Handle<WindowTag>;

// -----------------------------------------------------------------------------

namespace fileIO
{
    int writeBytes(const std::string &src, const std::string &dst);
    int writeImage(const core::FrameBuffer &src, const std::string &dst);
    std::string readBytes(const std::string &src);
} // namespace fileIO

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

    namespace
    {
        AssetData parseObj(const std::string &raw);
        AssetData parseMtl(const std::string &raw);
        AssetData parsePng(const std::string &raw);
        AssetData parsePpm(const std::string &raw);
        AssetData parseJson(const std::string &raw);
    } // namespace

    const std::string extensions[] = {"obj", "mtl", "png", "ppm", "json"};
    const parseFunc   parseFuncs[] = {parseObj, parseMtl, parsePng, parsePpm,
                                      parseJson};

    AssetData load(const std::string &path)
    {
        std::string bytes = fileIO::readBytes(path);
        AssetData   asset;

        // extension 판단 후 해당하는 parseFunc 호출
        // ...

        return asset;
    }

} // namespace asset

namespace window
{

    struct Manager
    {
        uint32_t id;
    };

    WindowHandle createInstance();
    void         showBuffer(const core::FrameBuffer &src);

} // namespace window

namespace core
{
    struct Mesh
    {
    };
    struct Texture
    {
    };
    struct Material
    {
    };
    struct FrameBuffer
    {
    };
    struct DepthBuffer
    {
    };
} // namespace core

namespace scene
{

    struct Object
    {
        MeshHandle     mesh;
        MaterialHandle mat;
        TextureHandle  tex;
        Vec3           pos;
        Vec3           rot;
        Vec3           scale;
    };

    struct Camera
    {
        Vec3 pos;
        Vec3 rot;
    };

    struct Scene
    {
        std::vector<Object> objects;
        Camera              camera;
        ObjectHandle        addObject(const Object &obj);
        void                setCamera(const Camera &cam);
    };

} // namespace scene

namespace resource
{
    class Manager
    {
      private:
        std::vector<core::Mesh>     meshes;
        std::vector<core::Texture>  textures;
        std::vector<core::Material> materials;

      public:
        core::Mesh    &getMesh(MeshHandle handle);
        core::Texture &getTexture(TextureHandle handle);
        uint32_t       addMesh(core::Mesh &m);
        uint32_t       addTexture(core::Texture &t);
    };

} // namespace resource

#include <functional>

namespace renderer
{
    // 차후 쉐이더 프로그램 추가
    // using VS = std::function<VSOut(const VSIn&)>
    // using FS = std::function<FSOut(const FSIn&)>
    class Renderer
    {
      private:
        // VS vs;
        // FS fs;
      public:
        int flags;
        int render(const scene::Scene &scn, core::FrameBuffer &fb,
                   core::DepthBuffer &db);
    };
} // namespace renderer

class App
{
  private:
    renderer::Renderer mainRenderer;
    // window::Manager    mainWindow;
    scene::Scene      mainScene;
    resource::Manager resourceManager;

    // 차후 배열로 변경
    core::FrameBuffer frameBuffer;
    core::DepthBuffer depthBuffer;

  public:
    void run()
    {
        // 1. process files (read & parse)
        // 실제로는 scene.json 안에 경로가 다 들어있음 (테스트용)
        mainScene = asset::load("scene.json");
        core::Mesh someMesh = asset::load("triangle.obj");
        resourceManager.addMesh(someMesh);

        // 2. render
        mainRenderer.render(mainScene, frameBuffer, depthBuffer);

        // 3. save & show framebuffer
        const std::string savedPath = "save.png";
        fileIO::writeImage(frameBuffer, savedPath);
    }
};
