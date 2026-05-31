#include "GlobalRenderState.h"

#include <Config/Config.h>

GlobalRenderState::GlobalRenderState(GPUContext &gpu)
    : RenderState(gpu)
{
    Config* config = GetService<Config>();

    canvasWidth = config->GetNamespace("Game").GetInt("Render.DefaultCanvasWidth", 640);
    canvasHeight = config->GetNamespace("Game").GetInt("Render.DefaultCanvasHeight", 360);

    // Nullptr initialize buffer
    for(int i = 0; i < worldBuffer.size(); ++i) {
        worldBuffer[i] = nullptr;
    }
}