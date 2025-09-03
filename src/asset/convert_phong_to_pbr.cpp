#include <tiny_obj_loader.h>
#include "asset.h"

namespace asset
{
    core::Material convertPhongToPBR(const tinyobj::material_t &tinyMat)
    {
        core::Material coreMat{};

        // 기본 색상 (Kd -> baseColor)
        coreMat.baseColor = math::Vec3{tinyMat.diffuse[0], tinyMat.diffuse[1], tinyMat.diffuse[2]};

        // 투명도/불투명도 (d 또는 Tr)
        coreMat.opacity = tinyMat.dissolve;

        // 금속성 추정: specular 값이 높고 diffuse가 낮으면 금속성으로 판단
        float avgSpecular =
            (tinyMat.specular[0] + tinyMat.specular[1] + tinyMat.specular[2]) / 3.0f;
        float avgDiffuse = (tinyMat.diffuse[0] + tinyMat.diffuse[1] + tinyMat.diffuse[2]) / 3.0f;

        if (avgSpecular > 0.9f && avgDiffuse < 0.1f)
        {
            coreMat.metallic = 1.0f;
            // 금속의 경우 baseColor를 specular 색상으로 대체
            coreMat.baseColor =
                math::Vec3{tinyMat.specular[0], tinyMat.specular[1], tinyMat.specular[2]};
        }
        else
        {
            coreMat.metallic = 0.0f;
        }

        // 거칠기: shininess(Ns)를 roughness로 변환
        // 공식: roughness = sqrt(2 / (shininess + 2))
        if (tinyMat.shininess > 0.0f)
        {
            coreMat.roughness = std::sqrt(2.0f / (tinyMat.shininess + 2.0f));
            coreMat.roughness = std::clamp(coreMat.roughness, 0.04f, 1.0f); // PBR 범위 제한
        }
        else
        {
            coreMat.roughness = 1.0f; // 완전히 거칠게
        }

        // 굴절률
        coreMat.ior = (tinyMat.ior > 0.0f) ? tinyMat.ior : 1.5f;

        // 발광 색상과 강도
        coreMat.emissive =
            math::Vec3{tinyMat.emission[0], tinyMat.emission[1], tinyMat.emission[2]};
        coreMat.emissiveIntensity =
            (coreMat.emissive.x + coreMat.emissive.y + coreMat.emissive.z) / 3.0f;

        // 기본 설정
        coreMat.doubleSided = false;
        coreMat.baseColorTex = TextureHandle{};
        coreMat.normalTex = TextureHandle{};
        coreMat.occlusionTex = TextureHandle{};

        return coreMat;
    }
} // namespace asset
