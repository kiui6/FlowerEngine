add_library(MoltenVK INTERFACE)

if(APPLE)
message(STATUS "Fetching MoltenVK 1.2.6")

include(FetchContent)

FetchContent_Declare(
    MoltenVKTar
    URL https://github.com/KhronosGroup/MoltenVK/releases/download/v1.2.6-rc2/MoltenVK-macos.tar
)

FetchContent_MakeAvailable(MoltenVKTar)

set(MOLTENVK_DYLIB "${moltenvktar_SOURCE_DIR}/MoltenVK/dylib/macOS/libMoltenVK.dylib")

target_sources(MoltenVK INTERFACE
    "${MOLTENVK_DYLIB}"
)
set_source_files_properties("${MOLTENVK_DYLIB}" PROPERTIES
    MACOSX_PACKAGE_LOCATION "Frameworks"
)

endif()