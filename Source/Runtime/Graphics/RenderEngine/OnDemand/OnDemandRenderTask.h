#pragma once

#include <concepts>

class OnDemandRenderTask
{
public:
    ~OnDemandRenderTask(){}

    virtual void Execute(){}
};

template<typename T>
concept OnDemandRenderTaskClass = std::is_base_of<OnDemandRenderTask, T>::value;