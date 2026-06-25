#pragma once

#include "Targets.h"
#include "Formats.h"
#include "Stages.h"

#include <string>

struct CompilationContext {
    std::string inputPath;
    std::string outputPath;
    CompilationTarget target = CompilationTarget::SPIRV;
    OutputFormat format = OutputFormat::Binary;
    ShaderStage stage = ShaderStage::Auto;
};