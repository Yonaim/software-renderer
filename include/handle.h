#pragma once
#include <cstdint>

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
