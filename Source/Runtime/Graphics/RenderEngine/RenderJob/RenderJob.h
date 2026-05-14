#pragma once

#include <concepts>

class RenderJob
{
public:
    virtual ~RenderJob() = default;

    virtual void Execute() = 0;
};

template<typename T>
concept RenderJobClass = std::is_base_of<RenderJob, T>::value;