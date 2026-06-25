#pragma once

#include <vector>
#include <string>

#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <spirv.hpp>

#include "Stages.h"

std::vector<uint32_t> CompileGLSLToSPIRV(const std::string& source, EShLanguage stage);