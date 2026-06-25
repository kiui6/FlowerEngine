#include <spirv.hpp>

#include "ArgParser.h"
#include "Context.h"

#include "SPIRVCompiler.h"
#include "WriteOut.h"
#include "Cross/Metal/SPIRVToMSL.h"
#include "Cross/Metal/MSLCompiler.h"
#include "Cross/DX/SPIRVToHLSL.h"
#include "Cross/DX/HLSLCompiler.h"

#include <iostream>
#include <exception>
#include <fstream>

void PrintUsage();
CompilationContext CreateContextFromArguments(const ArgParser& args);

int main(int argc, char* argv[]) {
    ArgParser args(argc, argv);

    if(!args.Has("input")) {
        PrintUsage();
        return 1;
    }

    CompilationContext context = CreateContextFromArguments(args);

    // Read input file
    std::ifstream file(context.inputPath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Failed to open input file: " << context.inputPath << "\n";
        return 2;
    }

    std::string inputSource = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    // Compile into SPIRV
    EShLanguage shaderStage;
    switch(context.stage) {
        case ShaderStage::Fragment:
            shaderStage = EShLanguage::EShLangFragment;
            break;
        case ShaderStage::Vertex:
            shaderStage = EShLanguage::EShLangVertex;
            break;
        default:
            if(context.inputPath.ends_with("vert")) {
                shaderStage = EShLanguage::EShLangVertex;
            }
            else if(context.inputPath.ends_with("vert")) {
                shaderStage = EShLanguage::EShLangFragment;
            } else {
                std::cerr << "Failed to deduce stage. Specify stage using --stage <vert/frag/comp/geom>\n";
                return 3;
            }
    }
    std::vector<uint32_t> spirv = CompileGLSLToSPIRV(inputSource, shaderStage);

    // If target != SPIRV, cross compile into target
    std::vector<uint8_t> finalBinary;
    if(context.target == CompilationTarget::SPIRV) {
        const uint8_t* bytePtr = reinterpret_cast<const uint8_t*>(spirv.data());
        size_t byteSize = spirv.size() * sizeof(uint32_t);

        finalBinary = std::vector<uint8_t>(bytePtr, bytePtr + byteSize);
    } else if(context.target == CompilationTarget::MSL){
        std::string mslSource = CrossCompileToMSL(spirv);

        const uint8_t* bytePtr = reinterpret_cast<const uint8_t*>(mslSource.data());
        size_t byteSize = mslSource.size();

        finalBinary = std::vector<uint8_t>(bytePtr, bytePtr + byteSize);
    } else if(context.target == CompilationTarget::Metallib){
        #ifdef PLATFORM_MAC
            std::string mslSource = CrossCompileToMSL(spirv);
            finalBinary = CompileMSLToMetallib(mslSource);
        #else
            std::cerr << "Metallib cross compilation is only available on MacOS";
            return 3;
        #endif
    } else if(context.target == CompilationTarget::HLSL){
        std::string hlslSource = CrossCompileToHLSL(spirv);

        const uint8_t* bytePtr = reinterpret_cast<const uint8_t*>(hlslSource.data());
        size_t byteSize = hlslSource.size();

        finalBinary = std::vector<uint8_t>(bytePtr, bytePtr + byteSize);
    } else if(context.target == CompilationTarget::DXBC){
        #ifdef HAS_DXC
            std::string hlslSource = CrossCompileToHLSL(spirv);
            finalBinary = CompileHLSLToDXBC(mslSource);
        #else
            std::cerr << "DXBC/DXIL cross compilation is only available if DXC is present on the system";
            return 3;
        #endif
    } else if(context.target == CompilationTarget::DXIL){
        #ifdef HAS_DXC
            std::string hlslSource = CrossCompileToHLSL(spirv);
            finalBinary = CompileHLSLToDXIL(mslSource);
        #else
            std::cerr << "DXBC/DXIL cross compilation is only available if DXC is present on the system";
            return 3;
        #endif
    }

    // Choose transformation operation based on output format & write output file
    try {
        if(context.format == OutputFormat::Binary) {
            WriteBinaryFile(context.outputPath, finalBinary);
        } else if (context.format == OutputFormat::CArray) {

        }
    } catch(const std::runtime_error& exc) {
        std::cerr << exc.what();
        return 4;
    }

    return 0;
}

void PrintUsage() {
    std::cerr << "Flower ShaderCompiler\n";
    std::cerr << "usage: ShaderCompiler --input <Path> [-flag] [--argument <value>]\n\n";
    std::cerr << "\t--input <Path> - GLSL source file for compilation\n\n";
    std::cerr << "\t--output <Path> - Path for output file\n\n";
    std::cerr << "\t--stage <vert/frag/comp/geom> - Specify shader stage\n\n";
    std::cerr << "\t--target <spirv/msl/metallib/dxil> - Specify target binary format for compilation\n\n";
    std::cerr << "\t--format <default/h> - format for output file\n";
    std::cerr << "\t\tdefault - output binary without any transformations\n";
    std::cerr << "\t\tcarray - output binary as c-array in a header file\n";
}

CompilationContext CreateContextFromArguments(const ArgParser& args)
{
    CompilationContext ctx;

    ctx.inputPath = *args.Get("input");

    std::optional<std::string_view> argTarget = args.Get("target");
    if(argTarget) {
        if(*argTarget == "spirv") ctx.target = CompilationTarget::SPIRV;
        else if(*argTarget == "msl") ctx.target = CompilationTarget::MSL;
        else if(*argTarget == "metallib") ctx.target = CompilationTarget::Metallib;
        else if(*argTarget == "hlsl") ctx.target = CompilationTarget::HLSL;
        else if(*argTarget == "dxbc") ctx.target = CompilationTarget::DXBC;
        else if(*argTarget == "dxil") ctx.target = CompilationTarget::DXIL;
    }

    std::optional<std::string_view> argFormat = args.Get("format");
    if(argFormat) {
        if(*argFormat == "default") ctx.format = OutputFormat::Binary;
        else if(*argFormat == "carray") ctx.format = OutputFormat::CArray;
    }

    std::optional<std::string_view> argOutput = args.Get("output");
    if(argOutput) {
        ctx.outputPath = *argOutput;
    } else {
        ctx.outputPath = ctx.inputPath;
        switch(ctx.target) {
            case CompilationTarget::SPIRV:
                ctx.outputPath += ".spv";
                break;
            case CompilationTarget::MSL:
                ctx.outputPath += ".msl";
                break;
            case CompilationTarget::Metallib:
                ctx.outputPath += ".metallib";
                break;
            case CompilationTarget::HLSL:
                ctx.outputPath += ".hlsl";
                break;
            case CompilationTarget::DXIL:
                ctx.outputPath += ".dxil";
                break;
            case CompilationTarget::DXBC:
                ctx.outputPath += ".dxbc";
                break;
            default:
                ctx.outputPath += ".bin";
        }
    }

    return ctx;
}