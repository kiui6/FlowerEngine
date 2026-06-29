cmake_minimum_required(VERSION 3.29)

set(ORGANIZATION_ID "com.flower" CACHE STRING "Organization ID")
set(PROJECT_NAME "flower" CACHE STRING "Project Name")

project ("Flower")

add_subdirectory("Tools")

set(BUILD_CONFIG "$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
set(SDL_STATIC ON)

#include("CMake/VulkanCompileShaders.cmake")
include("CMake/EnableLTO.cmake")
include("CMake/StripSymbols.cmake")
include("CMake/CompileShaders.cmake")
include("CMake/MoltenVK.cmake")
include(FetchContent)

message(STATUS "Fetching GLM 1.0.1")

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
  )
  
FetchContent_MakeAvailable(glm)
  
message(STATUS "Fetching JSON 3.12.0")

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
  )
  
FetchContent_MakeAvailable(json)
  
message(STATUS "Fetching SDL 3.4.4")

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
  GIT_TAG        release-3.4.4
)

FetchContent_MakeAvailable(sdl)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(disable_cxx_features INTERFACE)

target_compile_options(disable_cxx_features INTERFACE
    #   GCC / Clang
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Release>>:-fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti>
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Debug>>:-fno-exceptions -fno-rtti>
    #   MSVC
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/GR- /EHs- _HAS_EXCEPTIONS=0->
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/GR- /EHs- _HAS_EXCEPTIONS=0->
) 

# Third Party Sources
add_library(Imgui INTERFACE)
target_sources(Imgui INTERFACE "Source/ThirdParty/imgui/imgui_demo.cpp" "Source/ThirdParty/imgui/imgui_tables.cpp" 
             "Source/ThirdParty/imgui/imgui_draw.cpp" "Source/ThirdParty/imgui/imgui_widgets.cpp" 
             "Source/ThirdParty/imgui/imgui.cpp" "Source/ThirdParty/imgui/backends/imgui_impl_sdl3.cpp"
             "Source/ThirdParty/imgui/backends/imgui_impl_sdlgpu3.cpp")
target_include_directories(Imgui INTERFACE "Source/ThirdParty/imgui/")

set(SrcStbImage "Source/ThirdParty/stb/stb_image.cpp")

set(SrcLZ4 "Source/ThirdParty/lz4/lz4.c" "Source/ThirdParty/lz4/lz4file.c" "Source/ThirdParty/lz4/lz4frame.c" "Source/ThirdParty/lz4/lz4hc.c" "Source/ThirdParty/lz4/xxhash.c")

# Modules Sources
set(SrcApplication "Source/Runtime/Application/Application.cpp")

set(SrcService "Source/Runtime/Service/Service.cpp")

set(SrcLog "Source/Runtime/Log/Log.cpp")

set(SrcWorld "Source/Runtime/World/World.cpp" "Source/Runtime/World/Actor/Actor.cpp" "Source/Runtime/World/Actor/ReferenceRecordFactory.cpp" "Source/Runtime/World/SceneView.cpp")

set(SrcInput )

set(SrcEngine "Source/Runtime/Engine/Engine.cpp" "Source/Runtime/Engine/EngineDelegates.cpp")

set(SrcRecord "Source/Runtime/Record/Record.cpp" "Source/Runtime/Record/RecordLibrary.cpp" "Source/Runtime/Record/RecordPtr.cpp" "Source/Runtime/Record/RecordMerger.cpp" )

set(SrcAssets "Source/Runtime/Assets/Entity/EntityTemplate/EntityTemplateCompiler.cpp" "Source/Runtime/Assets/Entity/ComponentTemplate/EntityComponentTemplateLibrary.cpp")

set(SrcConfig "Source/Runtime/Config/Config.cpp")

set(SrcBlackboard "Source/Runtime/Blackboard/Blackboard.cpp")

set(SrcAsync "Source/Runtime/Async/AsyncService.cpp" "Source/Runtime/Async/TaskPool.cpp")

set(SrcData "Source/Runtime/Data/DataManager.cpp" "Source/Runtime/Data/DataReader.cpp" "Source/Runtime/Data/DirectoryView.cpp")

set(SrcAsset "Source/Runtime/Assets/AssetLibrary.cpp" "Source/Runtime/Assets/Asset.cpp" "Source/Runtime/Assets/RawAsset.cpp")

set(SrcPlatform "Source/Runtime/Platform/Platform.cpp" 

    # Abstract Input
    "Source/Runtime/Platform/Abstract/Input/PlatformInput.cpp"
    "Source/Runtime/Platform/Abstract/Input/RawInputDevice.cpp"

    # Abstract Filesystem
    "Source/Runtime/Platform/Abstract/Filesystem/PlatformFilesystem.cpp"
    "Source/Runtime/Platform/Abstract/Filesystem/File.cpp"

    # Abstract Window
    "Source/Runtime/Platform/Abstract/Window/Window.cpp"
  )

set(SrcLocale "Source/Runtime/Localization/LocaleManager.cpp" "Source/Runtime/Localization/Text.cpp")

set(SrcUtility "Source/Runtime/Utility/Memory/Arena.cpp" "Source/Runtime/Utility/PathBuilder.cpp")

set(SrcGC "Source/Runtime/GarbageCollector/GarbageCollector.cpp" "Source/Runtime/GarbageCollector/ReferenceCounter.cpp")

set(SrcMixin )

set(SrcPlugin "Source/Runtime/Plugins/PluginManager.cpp" "Source/Runtime/Plugins/PluginReader.cpp")

set(SrcEntry "Source/Runtime/Entry/EntryRecord.cpp")

set(SrcRender  
    # Render Engine
    "Source/Runtime/Graphics/RenderEngine/RenderEngine.cpp"
    "Source/Runtime/Graphics/RenderEngine/ResourceCompiler.cpp"
    "Source/Runtime/Graphics/RenderEngine/RenderUtils.cpp"

    # Render Engine - Render View
    "Source/Runtime/Graphics/RenderEngine/RenderView/RenderView.cpp"

    # Render Passes
    "Source/Runtime/Graphics/RenderPasses/OpaqueRenderPass/OpaqueRenderPass.cpp"
    "Source/Runtime/Graphics/RenderPasses/ReliefRenderPass.cpp"
    "Source/Runtime/Graphics/RenderPasses/LightingRenderPass.cpp"
    "Source/Runtime/Graphics/RenderPasses/UpscaleRenderPass/UpscaleRenderPass.cpp"
    "Source/Runtime/Graphics/RenderPasses/DebugUIRenderPass/DebugUIRenderPass.cpp"
    
    # Render States
    "Source/Runtime/Graphics/RenderStates/GlobalRenderState.cpp"
    "Source/Runtime/Graphics/RenderStates/UpscaleRenderState.cpp"

    # Render State Updates
    "Source/Runtime/Graphics/RenderStateUpdates/DebugUIStateUpdate.cpp"
    "Source/Runtime/Graphics/RenderStateUpdates/UpscaleStateUpdate.cpp"
    "Source/Runtime/Graphics/RenderStateUpdates/GlobalStateUpdate.cpp"
    
    # Buffer Attachment Update Handlers
    "Source/Runtime/Graphics/BufferAttachmentUpdateHandlers/WorldBufferAttachmentUpdateHandler.cpp"
  )

set(SrcGameFramework
    # Actors
    "Source/Runtime/GameFramework/Actors/SpriteActor/SpriteActor.cpp"

    # Records
    "Source/Runtime/GameFramework/Records/AtlasRecord/AtlasRecordFactory.cpp"

    # Assets
    "Source/Runtime/GameFramework/Assets/Texture2DAsset/Texture2DAsset.cpp"
)

set(Sources ${SrcLZ4} ${SrcStbImage} ${SrcApplication} ${SrcRender} ${SrcService} ${SrcLog}
    ${SrcWorld} ${SrcInput} ${SrcEngine} ${SrcRecord} ${SrcAsset} ${SrcConfig} ${SrcBlackboard} ${SrcAsync} ${SrcData}
    ${SrcPlatform} ${SrcLocale} ${SrcUtility} ${SrcGC} ${SrcGameFramework} ${SrcMixin} ${SrcPlugin}
    "Source/Runtime/Core/main.cpp" "Source/Runtime/Core/GuardedMain.cpp")

if(WIN32)
    set(WindowsPlatform "Source/Runtime/Platform/Windows/WindowsPlatform.cpp" 

      # Filesystem
      "Source/Runtime/Platform/Windows/Filesystem/WindowsMappedFile.cpp"

      # System
      "Source/Runtime/Platform/Windows/System/WindowsPlatformSystem.cpp"
    )
    
    set(Sources ${Sources} ${WindowsPlatform} "Resources/Platforms/Windows/resources.rc")
elseif(NOT APPLE AND UNIX)
    set(LinuxPlatform "Source/Runtime/Platform/Linux/LinuxPlatform.cpp" 

      # Filesystem
      "Source/Runtime/Platform/Linux/Filesystem/LinuxMappedFile.cpp"

      # System
      "Source/Runtime/Platform/Linux/System/LinuxPlatformSystem.cpp"
    )

    set(Sources ${Sources} ${LinuxPlatform})
elseif(APPLE)
    set(MacPlatform "Source/Runtime/Platform/Mac/MacPlatform.cpp" 

      # Filesystem
      "Source/Runtime/Platform/Mac/Filesystem/MacMappedFile.cpp"

      # System
      "Source/Runtime/Platform/Mac/System/MacPlatformSystem.cpp"
    )

    set(Sources ${Sources} ${MacPlatform})
endif()

add_library(Shared INTERFACE)
target_sources(Shared INTERFACE ${Sources})
target_link_libraries(Shared INTERFACE disable_cxx_features SDL3::SDL3-static glm-header-only nlohmann_json)
target_include_directories(Shared INTERFACE "Source/Runtime" "Source/ThirdParty")
target_include_directories(Shared INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_compile_features(Shared INTERFACE cxx_std_23)
add_dependencies(Shared FundamentalShaders)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(Shared INTERFACE Imgui) 
  target_compile_definitions(Shared INTERFACE HAS_DEBUGUI)
endif()

add_subdirectory("CMake/Targets")