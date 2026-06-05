#include "GlobalRenderState.h"

#include <Graphics/RenderEngine/RenderUtils.h>
#include <Config/Config.h>

#include <glm/gtc/matrix_transform.hpp>

GlobalRenderState::GlobalRenderState(GPUContext &gpu)
    : RenderState(gpu)
{
    Config* config = GetService<Config>();

    canvasWidth = config->GetNamespace("Game").GetInt("Render.DefaultCanvasWidth", 640);
    canvasHeight = config->GetNamespace("Game").GetInt("Render.DefaultCanvasHeight", 360);
}

GlobalRenderState::~GlobalRenderState()
{
}
