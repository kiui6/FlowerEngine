include(FetchContent)

message(STATUS "Fetching SPIRVCross")

FetchContent_Declare(
    SPIRVCross
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SPIRVCross)

message(STATUS "Fetching SPIRVHeaders 2026.2")

FetchContent_Declare(
    SPIRVHeaders
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SPIRVHeaders)

message(STATUS "Fetching GLSLang 16.3.0")

set(ENABLE_HLSL 0)
set(ENABLE_OPT 0)

FetchContent_Declare(
    glslang
    GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
    GIT_TAG 16.3.0
)
FetchContent_MakeAvailable(glslang)

message(STATUS "Searching for DXC")

find_package(dxc)
find_library(METAL_LIB Metal)

set(ShaderCompilerSources "Source/main.cpp" "Source/SPIRVCompiler.cpp" "Source/WriteOut.cpp" "Source/Cross/Metal/SPIRVToMSL.cpp" "Source/Cross/DX/SPIRVToHLSL.cpp")

add_executable(ShaderCompiler ${ShaderCompilerSources})
target_link_libraries(ShaderCompiler PRIVATE 
    glslang
    SPIRV
    OSDependent

    spirv-cross-core
    spirv-cross-glsl
    spirv-cross-msl
    spirv-cross-hlsl

    # Used for ShaderNode
    nlohmann_json
)

target_include_directories(ShaderCompiler PRIVATE ${glslang_SOURCE_DIR})

target_compile_features(ShaderCompiler PRIVATE cxx_std_23)
set_target_properties(ShaderCompiler PROPERTIES LINKER_LANGUAGE CXX)

if(METAL_LIB)
    find_library(FOUNDATION_LIB Foundation REQUIRED)

    target_sources(ShaderCompiler PRIVATE "Source/Cross/Metal/MSLCompiler.mm")
    target_link_libraries(ShaderCompiler PRIVATE ${METAL_LIB} ${FOUNDATION_LIB})
    target_compile_definitions(ShaderCompiler PRIVATE HAS_METAL)
    set_target_properties(ShaderCompiler PROPERTIES 
        OSX_DEPLOYMENT_TARGET "11"
    )
endif()
    
if(dxc_FOUND)
    target_sources(ShaderCompiler PRIVATE "Source/Cross/DX/HLSLCompiler.cpp")
    target_link_libraries(ShaderCompiler PRIVATE dxc::dxcompiler)
    target_compile_definitions(ShaderCompiler PRIVATE HAS_DXC)
endif()
