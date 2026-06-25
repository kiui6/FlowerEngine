add_library(MoltenVK INTERFACE)

# TODO: Temporal measure, move to metal with shadercross later.

if(APPLE)
message(STATUS "Fetching MoltenVK 1.4.1")

include(FetchContent)

FetchContent_Declare(
    MoltenVKTar
    URL https://github.com/KhronosGroup/MoltenVK/releases/download/v1.4.1/MoltenVK-macos.tar
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