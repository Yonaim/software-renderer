#include "window.h"
#include "logger.h"
#include <SDL2/SDL.h>

namespace window
{
    // channels: 3=RGB24, 4=RGBA8888
    bool Manager::create(const std::string &title, int w, int h, int ch)
    {
        destroy();
        width = w;
        height = h;
        channels = ch;

        // window
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, SDL_WINDOW_SHOWN);
        if (!window)
        {
            LOG_ERROR();
            return false;
        }

        // renderer
        renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
        {
            LOG_ERROR();
            destroy();
            return false;
        }

        // texture
        const Uint32 fmt = (channels == 3) ? SDL_PIXELFORMAT_RGB24 : SDL_PIXELFORMAT_RGBA8888;
        texture = SDL_CreateTexture(renderer, fmt, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (!texture)
        {
            LOG_ERROR();
            destroy();
            return false;
        }

        return true;
    }

    // upload pixels (CPU -> GPU)
    // https://wiki.libsdl.org/SDL2/SDL_LockTexture
    void Manager::render(const std::vector<uint8_t> &pixels)
    {
        uint8_t *target; // address of buffer
        int      pitch;  // GPU 상에서의 한 행의 bytes 수 (stride)

        // get and fill texture
        SDL_LockTexture(texture, nullptr, (void **)&target, &pitch);
        for (int y = 0; y < height; ++y)
            memcpy((uint8_t *)target + y * pitch, pixels.data() + y * width * channels,
                   width * channels);
        SDL_UnlockTexture(texture);

        // show
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    bool Manager::loop(const FrameCallback &onFrame, const EventCallback &onEvent)
    {
        if (!window || !renderer || !texture)
            return false;

        std::vector<uint8_t> pixels(width * height * channels);

        bool   running = true;
        Uint64 freq = SDL_GetPerformanceFrequency();
        Uint64 last = SDL_GetPerformanceCounter();

        while (running)
        {
            SDL_Event e;
            // 이벤트 처리
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    running = false;
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (onEvent)
                    onEvent(e);
            }

            // deltaTime 계산
            Uint64 now = SDL_GetPerformanceCounter();
            float  dt = (now - last) / freq;

            // frame callback
            if (!onFrame(dt, width, height, channels, pixels))
                break;

            // upload
            render(pixels);
        }
        return true;
    }

    void Manager::destroy()
    {
        if (texture)
            SDL_DestroyTexture(texture);
        if (renderer)
            SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
        SDL_Quit();
    }
}; // namespace window
