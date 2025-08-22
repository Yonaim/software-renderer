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

using MeshHandle = Handle<MeshTag>;
using TextureHandle = Handle<TextureTag>;
using MaterialHandle = Handle<MaterialTag>;
using ObjectHandle = Handle<ObjectTag>;

// -----------------------------------------------------------------------------

// stateless
namespace fileIO
{
    int saveRaw(const char *src, const char *dst);
    int saveBufferToImage(const core::FrameBuffer &src, const char *dst);
    std::string loadRaw(const char *src);

} // namespace fileIO

/*
[File Format]
- Mesh: obj, mtl
- Texture: png, ppm
- Scene: json
*/
namespace asset
{
    using AssetData = std::variant<core::Mesh, core::Texture, core::Material>;

    AssetData load(const std::string &path);

    namespace
    {
        core::Mesh     parseObject(const std::string &raw);
        core::Texture  parseTexture(const std::string &raw);
        core::Material parseMaterial(const std::string &raw);
    } // namespace
} // namespace asset

namespace window
{

    struct Manager
    {
        uint32_t id;
    };

    void showBuffer(const core::FrameBuffer &src);

} // namespace window

namespace core
{
    struct Mesh;
    struct Texture;
    struct Material;
    struct FrameBuffer;
    struct DepthBuffer;
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

namespace renderer
{
    struct RenderParams
    {
        uint32_t width;
        uint32_t height;
    };

    int render(const scene::Scene &scn, core::FrameBuffer &fb,
               core::DepthBuffer &db, const RenderParams &params = {});
} // namespace renderer
