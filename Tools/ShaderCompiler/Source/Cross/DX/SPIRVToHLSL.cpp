#include "SPIRVToHLSL.h"

std::string CrossCompileToHLSL(const std::vector<uint32_t> &spirv)
{
    spirv_cross::CompilerHLSL compiler(spirv);
    spirv_cross::CompilerHLSL::Options options;
    options.shader_model = 50; 
    options.support_nonzero_base_vertex_base_instance = true;
    options.enable_16bit_types = false; 
    compiler.set_hlsl_options(options);
    return compiler.compile();
}
