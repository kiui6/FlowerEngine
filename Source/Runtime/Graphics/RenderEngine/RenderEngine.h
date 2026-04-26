#pragma once

#include "RenderView/RenderView.h"

#include <array>

class RenderEngine
{
    std::array<RenderView, 2> m_renderViews;
    uint8_t m_currentFrame = 0;
public:
    RenderView& GetFrameRenderView() { return m_renderViews[(m_currentFrame + 1) % m_renderViews.size()];}

    void Render(float deltaTime, RenderView& renderView);
};