#pragma once

#include <spirv_cross.hpp>
#include <spirv_hlsl.hpp>

std::string CrossCompileToHLSL(const std::vector<uint32_t>& spirv);