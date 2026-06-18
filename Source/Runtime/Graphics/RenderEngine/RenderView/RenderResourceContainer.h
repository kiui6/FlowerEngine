#pragma once

#include <Utility/Hash.h>

#include "CompiledRenderResource.h"

#include <unordered_map>
#include <memory>


struct CompiledRenderResources {
    template <class ResT>
    using ResourceMap = std::unordered_map<uint64_t, std::shared_ptr<ResT>, GoldHash>;

    ResourceMap<CompiledTexture2DResource> texture2D;
};