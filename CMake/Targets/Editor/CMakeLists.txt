include(${PROJECT_SOURCE_DIR}/CMake/AddPlatformTarget.cmake)

set(EditorSources ) 

add_platform_target(Editor ${EditorSources})

target_link_libraries(Editor PRIVATE Imgui)

target_include_directories(Editor PRIVATE "${PROJECT_SOURCE_DIR}/Source/Editor")

target_compile_definitions(Editor PUBLIC EDITOR)
target_compile_definitions(Editor PUBLIC HAS_DEBUGUI)