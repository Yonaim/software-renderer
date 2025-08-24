#pragma once
#include "asset.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "window.h"

class App
{
  private:
    renderer::Renderer mainRenderer;
    window::Manager    mainWindow;
    scene::Scene       mainScene;
    resource::Manager  resourceManager;

    // 차후 배열로 변경
    core::FrameBuffer fb;
    core::DepthBuffer db;

  public:
    App(int width = 800, int height = 500)
        : fb(width, height), db(width, height) {};
    ~App() {};
    void run()
    {
        // 1. process files (read & parse)
        mainScene = asset::loadScene("assets/scene.json", resourceManager);

        // 2. render
        mainRenderer.render(mainScene, fb, db);

        // 3. save & show framebuffer
        fileIO::writePPM("output.ppm", fb.width, fb.height, fb.color);
    }
};
