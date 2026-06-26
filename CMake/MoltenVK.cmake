add_library(MoltenVK INTERFACE)

# TODO: Temporal measure, move to MoltenVK 1.2.6-rc2 & vulkan loader

if(APPLE)
    message(STATUS "Searching for MoltenVK")

    find_library(MOLTENVK_LIBRARY
        NAMES MoltenVK
        HINTS /opt/local/lib /usr/local/lib /usr/lib
        REQUIRED
    )

    if(NOT MOLTENVK_LIBRARY)
        message(STATUS "MoltenVK not found")
        message(STATUS "Fetching MoltenVK 1.4.1")

        include(FetchContent)
        FetchContent_Declare(
            MoltenVKTar
            URL https://github.com/KhronosGroup/MoltenVK/releases/download/v1.4.1/MoltenVK-macos.tar
        )

        FetchContent_MakeAvailable(MoltenVKTar)

        set(MOLTENVK_LIBRARY "${moltenvktar_SOURCE_DIR}/MoltenVK/dylib/macOS/libMoltenVK.dylib")
    else()
        message(STATUS "Found MoltenVK: ${MOLTENVK_LIBRARY}")
    endif()

    target_sources(MoltenVK INTERFACE
        "${MOLTENVK_LIBRARY}"
    )
    set_source_files_properties("${MOLTENVK_LIBRARY}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Frameworks"
    )

endif()