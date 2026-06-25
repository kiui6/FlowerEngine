#pragma once

#include <spirv_cross.hpp>
#include <spirv_msl.hpp>

std::string CrossCompileToMSL(const std::vector<uint32_t>& spirv);