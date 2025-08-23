#pragma once
#include "asset.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"

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
        mainScene = asset::loadScene("scene.json", resourceManager);

        // 2. render
        mainRenderer.render(mainScene, frameBuffer, depthBuffer);

        // 3. save & show framebuffer
        const std::string savedPath = "save.png";
        fileIO::writeImage(frameBuffer, savedPath);
    }
};
