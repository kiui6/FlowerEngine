#include "GlobalRenderState.h"

#include <Config/Config.h>

GlobalRenderState::GlobalRenderState(GPUContext &gpu)
    : RenderState(gpu)
{
    Config* config = GetService<Config>();

    canvasWidth = config->GetNamespace("Game").GetInt("Render.DefaultCanvasWidth", 640);
    canvasHeight = config->GetNamespace("Game").GetInt("Render.DefaultCanvasHeight", 360);
}