#pragma once
#include "core.h"
#include "scene.h"
#include <SDL2/SDL.h>
#include <cstdint>
#include <cstring>
#include <functional>

namespace window
{
#define X(name, msg) name,
    enum class ErrorCode
    {
#include "common_error.def"
#include "window_error.def"
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
#include "window_error.def"
        }
        return "Unknown error";
    }
#undef X

    // (deltaTime, width, height, channels, pixels)
    using FrameCallback = std::function<bool(float, int, int, int, std::vector<uint8_t>)>;
    using EventCallback = std::function<void(SDL_Event &)>;

    class Manager
    {
      private:
        SDL_Window   *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture  *texture = nullptr;
        int           width;
        int           height;

      public:
        int channels = 4;

        Manager() {}
        ~Manager() { destroy(); }

        bool create(const std::string &title, int w, int h, int ch = 4);
        void render(const std::vector<uint8_t> &pixels);
        bool loop(const FrameCallback &onFrame, const EventCallback &onEvent = {});
        void destroy();
    };
} // namespace window
