cmake_minimum_required(VERSION 3.21)

project ("Flower")

set(BUILD_CONFIG "$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
set(SDL_STATIC ON)

include("CMake/VulkanCompileShaders.cmake")
include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
  GIT_TAG        release-3.4.4
)

FetchContent_MakeAvailable(sdl glm json)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(no_rtti INTERFACE)

target_compile_options(no_rtti INTERFACE
    # MSVC (including clang-cl)
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:$<CXX_COMPILER_FRONTEND_VARIANT>,MSVC>>>:/GR->
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-fno-rtti>
    # Clang & AppleClang (non-MSVC mode)
    $<$<CXX_COMPILER_ID:AppleClang>:-fno-rtti>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:$<CXX_COMPILER_FRONTEND_VARIANT>,MSVC>>>:-fno-rtti>
)

# Third Party Sources
set(SrcStbImage "Source/ThirdParty/stb/stb_image.cpp")

set(SrcLZ4 "Source/ThirdParty/lz4/lz4.c" "Source/ThirdParty/lz4/lz4file.c" "Source/ThirdParty/lz4/lz4frame.c" "Source/ThirdParty/lz4/lz4hc.c" "Source/ThirdParty/lz4/xxhash.c")

set(SrcImgui "Source/ThirdParty/imgui/imgui_demo.cpp" "Source/ThirdParty/imgui/imgui_tables.cpp" 
             "Source/ThirdParty/imgui/imgui_draw.cpp" "Source/ThirdParty/imgui/imgui_widgets.cpp" 
             "Source/ThirdParty/imgui/imgui.cpp" "Source/ThirdParty/imgui/backends/imgui_impl_sdl3.cpp"
             "Source/ThirdParty/imgui/backends/imgui_impl_sdlgpu3.cpp")

# Modules Sources
set(SrcApplication "Source/Runtime/Application/Application.cpp")

set(SrcService "Source/Runtime/Service/Service.cpp")

set(SrcLog "Source/Runtime/Log/Log.cpp")

set(SrcWindow "Source/Runtime/Window/Window.cpp")

set(SrcWorld "Source/Runtime/World/World.cpp" "Source/Runtime/World/Actor/Actor.cpp" "Source/Runtime/World/Actor/ReferenceRecordFactory.cpp" "Source/Runtime/World/SceneView.cpp")

set(SrcInput "Source/Runtime/Input/InputManager.cpp")

set(SrcEngine "Source/Runtime/Engine/Engine.cpp" "Source/Runtime/Engine/EngineDelegates.cpp")

set(SrcRecord "Source/Runtime/Record/Record.cpp" "Source/Runtime/Record/RecordLibrary.cpp" "Source/Runtime/Record/RecordPtr.cpp" )

set(SrcAssets "Source/Runtime/Assets/Entity/EntityTemplate/EntityTemplateCompiler.cpp" "Source/Runtime/Assets/Entity/ComponentTemplate/EntityComponentTemplateLibrary.cpp")

set(SrcConfig "Source/Runtime/Config/Config.cpp")

set(SrcBlackboard "Source/Runtime/Blackboard/Blackboard.cpp")

set(SrcAsync "Source/Runtime/Async/AsyncService.cpp" "Source/Runtime/Async/TaskPool.cpp")

set(SrcData "Source/Runtime/Data/DataManager.cpp" "Source/Runtime/Data/DataReader.cpp")

set(SrcAsset "Source/Runtime/Assets/AssetLibrary.cpp" "Source/Runtime/Assets/Asset.cpp" "Source/Runtime/Assets/RawAsset.cpp")

set(SrcPlatform "Source/Runtime/Platform/Platform.cpp" "Source/Runtime/Platform/File.cpp")

set(SrcLocale "Source/Runtime/Localization/LocaleManager.cpp" "Source/Runtime/Localization/Text.cpp")

set(SrcUtility "Source/Runtime/Utility/Arena.cpp" "Source/Runtime/Utility/PathBuilder.cpp")

set(SrcGC "Source/Runtime/GarbageCollector/GarbageCollector.cpp" "Source/Runtime/GarbageCollector/ReferenceCounter.cpp")

set(SrcMixin )

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
    "Source/Runtime/Graphics/RenderPasses/UpscaleRenderPass.cpp"
    "Source/Runtime/Graphics/RenderPasses/DebugUIRenderPass/DebugUIRenderPass.cpp"
    
    # Render States
    "Source/Runtime/Graphics/RenderStates/GlobalRenderState.cpp"

    # Render State Updates
    "Source/Runtime/Graphics/RenderStateUpdates/DebugUIStateUpdate.cpp"
  )

set(SrcGameFramework
    # Actors
    "Source/Runtime/GameFramework/Actors/SpriteActor/SpriteActor.cpp"

    # Records
    "Source/Runtime/GameFramework/Records/AtlasRecord/AtlasRecordFactory.cpp"

    # Assets
    "Source/Runtime/GameFramework/Assets/Texture2DAsset/Texture2DAsset.cpp"
)

set(Sources ${SrcLZ4} ${SrcStbImage} ${SrcApplication} ${SrcRender} ${SrcService} ${SrcLog} ${SrcWindow}
    ${SrcWorld} ${SrcInput} ${SrcEngine} ${SrcRecord} ${SrcAsset} ${SrcConfig} ${SrcBlackboard} ${SrcAsync} ${SrcData}
    ${SrcPlatform} ${SrcLocale} ${SrcUtility} ${SrcGC} ${SrcGameFramework} ${SrcMixin}
    "Source/Runtime/Core/main.cpp" "Source/Runtime/Core/GuardedMain.cpp")

if(WIN32)
    set(Sources WIN32 ${Sources} "Editor/Platforms/Windows/resources.rc" "Source/Runtime/Platform/Windows/WindowsPlatform.cpp" "Source/Runtime/Platform/Windows/WindowsMappedFile.cpp")
endif()
if(CMAKE_HOST_SYSTEM MATCHES Linux)
  set(Sources ${Sources} "Source/Runtime/Platform/Linux/LinuxPlatform.cpp")
endif()

# Only include imgui in debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Sources ${Sources} ${SrcImgui}) 
endif()
# Game

add_executable(Game ${Sources})

target_link_libraries(Game no_rtti SDL3::SDL3-static glm-header-only nlohmann_json)

target_include_directories(Game PRIVATE "Source/Runtime")
target_include_directories(Game PRIVATE "Source/ThirdParty")

set_property(TARGET Game PROPERTY CXX_STANDARD 23)

target_compile_options(Game PRIVATE
#   GCC / Clang
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Release>>:-fno-exceptions>
#   MSVC
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/EHs _HAS_EXCEPTIONS=0->
)

# Include HAS_DEBUGUI define to the game in Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(Game PUBLIC HAS_DEBUGUI)
endif()

# Editor

set(EditorSources ${SrcImgui}) 

add_executable(Editor ${Sources} ${EditorSources})

target_link_libraries(Editor no_rtti SDL3::SDL3-static glm-header-only nlohmann_json)

target_include_directories(Editor PRIVATE "Source/Runtime" "Source/Editor")

target_include_directories(Editor PRIVATE "Source/ThirdParty")

target_compile_definitions(Editor PUBLIC EDITOR)
target_compile_definitions(Editor PUBLIC HAS_DEBUGUI)

set_property(TARGET Editor PROPERTY CXX_STANDARD 23)

# Address Sanitizer Builds

add_executable(EditorASAN ${Sources} ${EditorSources})

target_link_libraries(EditorASAN no_rtti SDL3::SDL3-static glm-header-only nlohmann_json)

target_include_directories(EditorASAN PRIVATE "Source/Runtime" "Source/Editor")

target_include_directories(EditorASAN PRIVATE "Source/ThirdParty")

target_compile_definitions(EditorASAN PUBLIC EDITOR)
target_compile_definitions(EditorASAN PUBLIC HAS_DEBUGUI)

set_property(TARGET EditorASAN PROPERTY CXX_STANDARD 23)

if(MSVC)
message(STATUS "ASAN: Detected MSVC")
  target_compile_options(EditorASAN PUBLIC "/MTd" "/fsanitize=address" "/Zi" "/sdl" "/permissive-") 
  target_link_options(EditorASAN PUBLIC "/INCREMENTAL:NO")
  target_link_options(EditorASAN PUBLIC "/subsystem:console")
else()
  message(STATUS "ASAN: Detected GNU or LLVM")
  target_compile_options(EditorASAN PUBLIC "-fsanitize=address") 
  target_link_options(EditorASAN PUBLIC "-fsanitize=address")
endif()
#set(CMAKE_EXE_LINKER_FLAGS "/subsystem:console ${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")

# Shaders
add_shaders(Shaders "Shaders/FullscreenTriangle/FullscreenTriangle.vert" "Shaders/FullscreenTriangle/FullscreenTriangle.frag")