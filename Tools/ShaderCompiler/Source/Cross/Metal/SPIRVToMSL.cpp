#include "SPIRVToMSL.h"

std::string CrossCompileToMSL(const std::vector<uint32_t> &spirv)
{
    spirv_cross::CompilerMSL compiler(spirv);
    spirv_cross::CompilerMSL::Options options;
    options.platform = spirv_cross::CompilerMSL::Options::macOS;
    options.msl_version = spirv_cross::CompilerMSL::Options::make_msl_version(2, 0);
    compiler.set_msl_options(options);
    return compiler.compile();
}
