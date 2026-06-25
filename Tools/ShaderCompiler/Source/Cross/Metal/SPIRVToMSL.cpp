#include "SPIRVToMSL.h"

std::string CrossCompileToMSL(const std::vector<uint32_t> &spirv, ShaderStage stage)
{
    spirv_cross::CompilerMSL compiler(spirv);
    //compiler.set_entry_point("main", stage == ShaderStage::Fragment ? spirv_cross::ExecutionModelFragment : spirv_cross::ExecutionModelVertex);

    spirv_cross::CompilerMSL::Options options;
    options.platform = spirv_cross::CompilerMSL::Options::macOS;
    options.msl_version = spirv_cross::CompilerMSL::Options::make_msl_version(2, 0);
    compiler.set_msl_options(options);
    return compiler.compile();
}
