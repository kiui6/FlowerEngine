#include "GlobalRenderState.h"

#include <Config/Config.h>

GlobalRenderState::GlobalRenderState(GPUContext &gpu)
    : RenderState(gpu)
{
    Config* config = GetService<Config>();

    canvasWidth = config->GetNamespace("Game").GetInt("Render.PixelPerfect.CanvasWidth", 320);
    canvasHeight = config->GetNamespace("Game").GetInt("Render.PixelPerfect.CanvasHeight", 180);
}