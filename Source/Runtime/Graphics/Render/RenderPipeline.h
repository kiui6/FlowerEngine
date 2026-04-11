#pragma once

#include <Utility/Concepts.h>
#include "RenderPipelineStage.h"

#include <array>

class RenderPipeline
{
protected:
    std::array<PipelineStage*, RENDER_PIPELINE_STAGE_MAX> m_PipelineStages;

    template<Derived<PipelineStage> T> 
    void RegisterStage()
	{
		T* pPipelineStage = new T(*this);
		m_PipelineStages[T::StageID] = pPipelineStage;
	}
};