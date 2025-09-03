#include <simdjson.h>
#include <stb_image.h>
#include <tiny_obj_loader.h>
#include "asset.h"

namespace asset
{
    core::Material convertPhongToPBR(const tinyobj::material_t &tinyMat);

    Result<core::Mesh> parser::obj(std::string_view text)
    {
        tinyobj::ObjReaderConfig cfg;
        cfg.triangulate = true; // force to triangle
        cfg.vertex_color = false;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromString(std::string(text), /*mtl_text=*/""))
            return std::unexpected(ErrorCode::InvalidFormat);

        const auto &attrib = reader.GetAttrib();
        const auto &shapes = reader.GetShapes();
        const auto &materials = reader.GetMaterials(); // 없을 수도 있음

        // 출력 메시
        core::Mesh out{};
        out.vertices.clear();
        out.indices.clear();
        out.subs.clear();

        std::unordered_map<std::string, uint32_t> ordinal; // {material_name, count}
        ordinal.clear();

        auto startNewSubmesh = [&](const std::string &matName, MaterialHandle matHandle,
                                   uint32_t idxStart) -> core::Submesh
        {
            const uint32_t k = ++ordinal[matName]; // 1,2,3,...

            std::string tag = matName + "_#" + std::to_string(k);

            core::Submesh sm{};
            sm.groupId = tag;
            sm.material = matHandle;
            sm.idxStart = idxStart;
            sm.idxEnd = idxStart;
            return sm;
        };

        auto idxToVertex = [&](const tinyobj::index_t &idx) -> core::Vertex
        {
            core::Vertex v{};

            if (idx.vertex_index >= 0)
            {
                const int   vi = idx.vertex_index;
                const float x = attrib.vertices[3 * vi + 0];
                const float y = attrib.vertices[3 * vi + 1];
                const float z = attrib.vertices[3 * vi + 2];
                v.position = math::Vec3{x, y, z};
            }

            if (idx.normal_index >= 0 && !attrib.normals.empty())
            {
                const int   ni = idx.normal_index;
                const float nx = attrib.normals[3 * ni + 0];
                const float ny = attrib.normals[3 * ni + 1];
                const float nz = attrib.normals[3 * ni + 2];

                if constexpr (requires(core::Vertex a) { a.normal = math::Vec3{}; })
                    v.normal = math::Vec3{nx, ny, nz};
                else if constexpr (requires(core::Vertex a) { a.normal = math::Vec3{}; })
                    v.normal = math::Vec3{nx, ny, nz};
            }

            if (idx.texcoord_index >= 0 && !attrib.texcoords.empty())
            {
                const int   ti = idx.texcoord_index;
                const float u = attrib.texcoords[2 * ti + 0];
                const float v2 = attrib.texcoords[2 * ti + 1];

                if constexpr (requires(core::Vertex a) { a.uv = math::Vec2{}; })
                    v.uv = math::Vec2{u, v2};
                else if constexpr (requires(core::Vertex a) { a.uv = math::Vec2{}; })
                    v.uv = math::Vec2{u, v2};
                else if constexpr (requires(core::Vertex a) { a.uv = math::Vec2{}; })
                    v.uv = math::Vec2{u, v2};
            }

            return v;
        };

        // 현재 진행중인 서브메시 유무
        bool          has_open_submesh = false;
        core::Submesh cur{};

        auto flush_current_submesh = [&]()
        {
            if (has_open_submesh)
            {
                cur.idxEnd = static_cast<uint32_t>(out.indices.size()); // exclusive
                out.subs.push_back(cur);
                has_open_submesh = false;
            }
        };

        // 면 단위로 진행하며 머티리얼을 기준으로 서브 메쉬 분리
        for (const auto &sh : shapes)
        {
            const auto &msh = sh.mesh;
            size_t      face_offset = 0;
            for (size_t f = 0; f < msh.num_face_vertices.size(); ++f)
            {
                const int   fv = msh.num_face_vertices[f];
                std::string matName = "__default__";
                int         mat_id = -1;
                if (!msh.material_ids.empty())
                {
                    mat_id = msh.material_ids[f];
                    if (0 <= mat_id && mat_id < static_cast<int>(materials.size()))
                    {
                        matName = materials[mat_id].name;
                        if (matName.empty())
                            matName = "__unknown__";
                    }
                }

                // 새로운 서브 메쉬 시작 (처음 or 머티리얼 변경)
                static std::string prevMatName = "";
                if (!has_open_submesh || matName != prevMatName)
                {
                    flush_current_submesh();
                    cur = startNewSubmesh(matName, MaterialHandle{},
                                          static_cast<uint32_t>(out.indices.size()));
                    has_open_submesh = true;
                    prevMatName = matName;
                }

                // 면의 각 버텍스 처리
                for (int v = 0; v < fv; ++v)
                {
                    const tinyobj::index_t idx = msh.indices[face_offset + v];
                    const core::Vertex     vert = idxToVertex(idx);
                    out.vertices.push_back(vert);
                    out.indices.push_back(static_cast<uint32_t>(out.vertices.size() - 1));
                }

                face_offset += fv;
            }
        }
        // 마지막 서브메시 flush
        flush_current_submesh();
        return out;
    }

    Result<parser::MaterialEntries> parser::mtl(std::string_view text)
    {
        std::map<std::string, int>       material_map;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn;
        std::string                      err;
        std::istringstream               ss{std::string(text)};

        // MTL 파일 파싱
        tinyobj::LoadMtl(&material_map, &materials, &ss, &warn, &err);
        bool success = (warn == "");

        if (!success || !err.empty())
        {
            std::cerr << "MTL load error: " << err << std::endl;
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        if (!warn.empty())
        {
            std::cerr << "MTL load warning: " << warn << std::endl;
            // 경고는 있지만 계속 진행
        }

        parser::MaterialEntries entries;
        entries.clear();

        // 각 tinyobj material을 core::Material로 변환
        for (const auto &tinyMat : materials)
        {
            // PBR 변환 함수 호출
            core::Material coreMat = convertPhongToPBR(tinyMat);

            // material name 처리
            std::string materialName = tinyMat.name;
            if (materialName.empty())
            {
                materialName = "__unnamed_" + std::to_string(entries.size());
            }

            // entries에 추가
            entries[materialName] = coreMat;
        }

        return entries;
    }

    Result<parser::SceneConfig> parser::json(std::string_view text)
    {
        simdjson::dom::parser parser;
        auto                  parse_result = parser.parse(text);

        if (parse_result.error() != simdjson::SUCCESS)
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        simdjson::dom::element doc = parse_result.value();

        parser::SceneConfig config;

        try
        {
            // Scene name
            if (doc["name"].error() == simdjson::SUCCESS)
            {
                std::string_view name_view;
                if (doc["name"].get(name_view) == simdjson::SUCCESS)
                {
                    config.name = std::string(name_view);
                }
            }

            // Camera
            auto camera_elem = doc["camera"];
            if (camera_elem.error() == simdjson::SUCCESS)
            {
                // Position
                auto pos_array = camera_elem["pos"].get_array();
                if (pos_array.error() == simdjson::SUCCESS)
                {
                    auto it = pos_array.value().begin();
                    config.camera.pos.x = double(*it++);
                    config.camera.pos.y = double(*it++);
                    config.camera.pos.z = double(*it);
                }

                // Rotation
                auto rot_array = camera_elem["rot"].get_array();
                if (rot_array.error() == simdjson::SUCCESS)
                {
                    auto it = rot_array.value().begin();
                    config.camera.rot.x = double(*it++);
                    config.camera.rot.y = double(*it++);
                    config.camera.rot.z = double(*it);
                }

                // FOV, near, far
                double fov, near, far;
                if (camera_elem["fov"].get(fov) == simdjson::SUCCESS)
                {
                    config.camera.fov = static_cast<float>(fov);
                }
                if (camera_elem["near"].get(near) == simdjson::SUCCESS)
                {
                    config.camera.near = static_cast<float>(near);
                }
                if (camera_elem["far"].get(far) == simdjson::SUCCESS)
                {
                    config.camera.far = static_cast<float>(far);
                }
            }

            // Lights
            auto lights_array = doc["lights"].get_array();
            if (lights_array.error() == simdjson::SUCCESS)
            {
                for (auto light_elem : lights_array.value())
                {
                    parser::LightConfig light;

                    // ID
                    std::string_view id_view;
                    if (light_elem["id"].get(id_view) == simdjson::SUCCESS)
                    {
                        light.id = std::string(id_view);
                    }

                    // Type
                    std::string_view type_view;
                    if (light_elem["type"].get(type_view) == simdjson::SUCCESS)
                    {
                        std::string type_str(type_view);
                        if (type_str == "directional")
                        {
                            light.type = scene::LightType::Directional;
                        }
                        else if (type_str == "point")
                        {
                            light.type = scene::LightType::Point;
                        }
                        else if (type_str == "spot")
                        {
                            light.type = scene::LightType::Spot;
                        }
                    }

                    // Direction (for directional lights)
                    auto dir_array = light_elem["dir"].get_array();
                    if (dir_array.error() == simdjson::SUCCESS)
                    {
                        auto it = dir_array.value().begin();
                        light.directional.dir.x = double(*it++);
                        light.directional.dir.y = double(*it++);
                        light.directional.dir.z = double(*it);
                    }

                    // Position (for point lights)
                    auto pos_array = light_elem["pos"].get_array();
                    if (pos_array.error() == simdjson::SUCCESS)
                    {
                        auto it = pos_array.value().begin();
                        light.point.pos.x = double(*it++);
                        light.point.pos.y = double(*it++);
                        light.point.pos.z = double(*it);
                    }

                    // Color
                    auto color_array = light_elem["color"].get_array();
                    if (color_array.error() == simdjson::SUCCESS)
                    {
                        auto it = color_array.value().begin();
                        light.color.x = double(*it++);
                        light.color.y = double(*it++);
                        light.color.z = double(*it);
                    }

                    // Intensity
                    double intensity;
                    if (light_elem["intensity"].get(intensity) == simdjson::SUCCESS)
                    {
                        light.intensity = static_cast<float>(intensity);
                    }

                    // Range (for point lights)
                    double range;
                    if (light_elem["range"].get(range) == simdjson::SUCCESS)
                    {
                        light.point.range = static_cast<float>(range);
                    }

                    // Cast shadow
                    bool cast_shadow;
                    if (light_elem["cast_shadow"].get(cast_shadow) == simdjson::SUCCESS)
                    {
                        light.castShadow = cast_shadow;
                    }

                    config.lights.push_back(light);
                }
            }

            // Materials
            auto materials_array = doc["materials"].get_array();
            if (materials_array.error() == simdjson::SUCCESS)
            {
                for (auto mat_elem : materials_array.value())
                {
                    parser::MaterialConfig mat;

                    std::string_view id_view, file_view, name_view;
                    if (mat_elem["id"].get(id_view) == simdjson::SUCCESS)
                    {
                        mat.id = id_view;
                    }
                    if (mat_elem["file"].get(file_view) == simdjson::SUCCESS)
                    {
                        mat.file = file_view;
                    }
                    if (mat_elem["name"].get(name_view) == simdjson::SUCCESS)
                    {
                        mat.name = name_view;
                    }

                    config.materials.push_back(mat);
                }
            }

            // Geometries
            auto geometries_array = doc["geometries"].get_array();
            if (geometries_array.error() == simdjson::SUCCESS)
            {
                for (auto geo_elem : geometries_array.value())
                {
                    parser::GeometryConfig geo;

                    std::string_view id_view, file_view;
                    if (geo_elem["id"].get(id_view) == simdjson::SUCCESS)
                    {
                        geo.id = id_view;
                    }
                    if (geo_elem["file"].get(file_view) == simdjson::SUCCESS)
                    {
                        geo.file = file_view;
                    }

                    config.geometries.push_back(geo);
                }
            }

            // Objects
            auto objects_array = doc["objects"].get_array();
            if (objects_array.error() == simdjson::SUCCESS)
            {
                for (auto obj_elem : objects_array.value())
                {
                    parser::ObjectConfig obj;

                    std::string_view id_view, mesh_view;
                    if (obj_elem["id"].get(id_view) == simdjson::SUCCESS)
                    {
                        obj.id = id_view;
                    }
                    if (obj_elem["mesh"].get(mesh_view) == simdjson::SUCCESS)
                    {
                        obj.meshId = mesh_view;
                    }

                    // Transform
                    auto transform_elem = obj_elem["transform"];
                    if (transform_elem.error() == simdjson::SUCCESS)
                    {
                        // Position
                        auto pos_array = transform_elem["pos"].get_array();
                        if (pos_array.error() == simdjson::SUCCESS)
                        {
                            auto it = pos_array.value().begin();
                            obj.pos.x = double(*it++);
                            obj.pos.y = double(*it++);
                            obj.pos.z = double(*it);
                        }

                        // Rotation
                        auto rot_array = transform_elem["rot"].get_array();
                        if (rot_array.error() == simdjson::SUCCESS)
                        {
                            auto it = rot_array.value().begin();
                            obj.rot.x = double(*it++);
                            obj.rot.y = double(*it++);
                            obj.rot.z = double(*it);
                        }

                        // Scale
                        auto scale_array = transform_elem["scale"].get_array();
                        if (scale_array.error() == simdjson::SUCCESS)
                        {
                            auto it = scale_array.value().begin();
                            obj.scale.x = double(*it++);
                            obj.scale.y = double(*it++);
                            obj.scale.z = double(*it);
                        }
                    }

                    config.objects.push_back(obj);
                }
            }
        }
        catch (...)
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        return config;
    }

    Result<parser::ImageBuffer> parser::png(std::span<const std::byte> bytes)
    {
        int w, h, original_channels;

        // stb_image로 PNG 디코딩 (RGBA로 강제 변환)
        unsigned char *decoded =
            stbi_load_from_memory(reinterpret_cast<const unsigned char *>(bytes.data()),
                                  static_cast<int>(bytes.size()), &w, &h, &original_channels, 4);

        if (!decoded)
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        parser::ImageBuffer imgBuf;
        imgBuf.width = w;
        imgBuf.height = h;
        imgBuf.format = parser::PixelFormat::RGBA8;
        imgBuf.transFunc = parser::TransferFunc::NonLinear; // PNG는 보통 sRGB 감마

        // RGBA 픽셀 데이터 복사
        const size_t total_bytes = static_cast<size_t>(w * h * 4);
        imgBuf.pixels.resize(total_bytes);
        std::memcpy(imgBuf.pixels.data(), decoded, total_bytes);

        // stb_image 메모리 해제
        stbi_image_free(decoded);
        return imgBuf;
    }

    Result<parser::ImageBuffer> parser::ppm(std::span<const std::byte> bytes)
    {
        if (bytes.size() < 2 || static_cast<char>(bytes[0]) != 'P')
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        // PPM 형식 확인 (P3 = ASCII RGB, P6 = Binary RGB)
        char format_type = static_cast<char>(bytes[1]);
        if (format_type != '3' && format_type != '6')
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        // 바이트를 문자열로 변환 (헤더 파싱용)
        std::string        content(reinterpret_cast<const char *>(bytes.data()), bytes.size());
        std::istringstream iss(content);

        std::string magic;
        int         width, height, max_val;

        // 헤더 파싱
        if (!(iss >> magic >> width >> height >> max_val))
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        if (width <= 0 || height <= 0 || max_val <= 0 || max_val > 65535)
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        parser::ImageBuffer imgBuf;
        imgBuf.width = width;
        imgBuf.height = height;
        imgBuf.transFunc = parser::TransferFunc::Linear; // PPM은 보통 선형

        // 픽셀 데이터 읽기 위치 찾기
        size_t header_end = iss.tellg();
        if (header_end == std::string::npos)
        {
            return std::unexpected(ErrorCode::InvalidFormat);
        }

        // 개행 문자 건너뛰기
        while (header_end < bytes.size() &&
               (bytes[header_end] == std::byte{'\n'} || bytes[header_end] == std::byte{'\r'} ||
                bytes[header_end] == std::byte{' '}))
        {
            header_end++;
        }

        const size_t pixel_count = static_cast<size_t>(width * height);

        if (format_type == '6')
        { // Binary format
            if (max_val <= 255)
            {
                imgBuf.format = parser::PixelFormat::RGB8;
                imgBuf.pixels.resize(pixel_count * 3);

                const size_t data_size = pixel_count * 3;
                if (header_end + data_size > bytes.size())
                {
                    return std::unexpected(ErrorCode::InvalidFormat);
                }

                std::memcpy(imgBuf.pixels.data(),
                            reinterpret_cast<const uint8_t *>(bytes.data()) + header_end,
                            data_size);
            }
            else
            {
                imgBuf.format = parser::PixelFormat::RGB16;
                imgBuf.pixels.resize(pixel_count * 6); // 16비트 * 3채널

                const size_t data_size = pixel_count * 6;
                if (header_end + data_size > bytes.size())
                {
                    return std::unexpected(ErrorCode::InvalidFormat);
                }

                std::memcpy(imgBuf.pixels.data(),
                            reinterpret_cast<const uint8_t *>(bytes.data()) + header_end,
                            data_size);
            }
        }
        else
        { // ASCII format (P3)
            imgBuf.format = parser::PixelFormat::RGB8;
            imgBuf.pixels.resize(pixel_count * 3);

            // ASCII 데이터 파싱
            std::string remaining_content(reinterpret_cast<const char *>(bytes.data()) + header_end,
                                          bytes.size() - header_end);
            std::istringstream pixel_iss(remaining_content);

            for (size_t i = 0; i < pixel_count * 3; ++i)
            {
                int value;
                if (!(pixel_iss >> value))
                {
                    return std::unexpected(ErrorCode::InvalidFormat);
                }

                // 값 정규화 및 클램핑
                if (max_val != 255)
                {
                    value = (value * 255) / max_val;
                }
                value = std::clamp(value, 0, 255);
                imgBuf.pixels[i] = static_cast<uint8_t>(value);
            }
        }

        return imgBuf;
    }
} // namespace asset