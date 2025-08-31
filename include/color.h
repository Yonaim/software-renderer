#pragma once
#include <algorithm>
#include <cmath>
#include "math/vec.h" // math::Vec3, Vec4 사용 시

namespace color {

// sRGB [0,1] -> Linear [0,1]
inline float srgbToLinear(float s) noexcept {
    s = std::clamp(s, 0.0f, 1.0f);
    return (s <= 0.04045f) ? (s / 12.92f)
                           : std::pow((s + 0.055f) / 1.055f, 2.4f);
}

// Linear [0,1] -> sRGB [0,1]
inline float linearToSrgb(float l) noexcept {
    l = std::clamp(l, 0.0f, 1.0f);
    return (l <= 0.0031308f) ? (12.92f * l)
                             : (1.055f * std::pow(l, 1.0f/2.4f) - 0.055f);
}

// 벡터 오버로드
inline math::Vec3 srgbToLinear(math::Vec3 c) noexcept {
    return { srgbToLinear(c.x), srgbToLinear(c.y), srgbToLinear(c.z) };
}
inline math::Vec4 srgbToLinear(math::Vec4 c) noexcept {
    return { srgbToLinear(c.x), srgbToLinear(c.y), srgbToLinear(c.z), c.w };
}
inline math::Vec3 linearToSrgb(math::Vec3 c) noexcept {
    return { linearToSrgb(c.x), linearToSrgb(c.y), linearToSrgb(c.z) };
}
inline math::Vec4 linearToSrgb(math::Vec4 c) noexcept {
    return { linearToSrgb(c.x), linearToSrgb(c.y), linearToSrgb(c.z), c.w };
}

} // namespace color
