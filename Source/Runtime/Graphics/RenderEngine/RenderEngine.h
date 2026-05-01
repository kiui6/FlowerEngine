#pragma once

#include "RenderView/RenderView.h"
#include "RenderPass/RenderPass.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <map>
#include <array>

class RenderEngine
{
    std::array<RenderView, 2> m_renderViews;
    uint8_t m_currentFrame = 0;

    std::array<std::unique_ptr<RenderPass>, (uint32_t)RenderPassType::MAX> m_renderPasses;

    std::unordered_map<uint64_t, std::shared_ptr<RenderResource>> m_resources;
public:
    RenderView& GetFrameRenderView() { return m_renderViews[(m_currentFrame + 1) % m_renderViews.size()];}

    void Initialize();

    void Render(float deltaTime, RenderView& renderView);
};