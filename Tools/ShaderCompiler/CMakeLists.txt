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

set(ShaderCompilerSources "Source/main.cpp" "Source/ArgParser.cpp")

add_executable(ShaderCompiler ${ShaderCompilerSources})
target_link_libraries(ShaderCompiler PRIVATE 
    c++

    glslang
    SPIRV
    OSDependent
    
    spirv-cross-core
    spirv-cross-glsl
    spirv-cross-msl
    spirv-cross-hlsl
)