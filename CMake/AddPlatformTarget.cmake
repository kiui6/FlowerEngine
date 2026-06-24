function(add_platform_target TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})

    target_link_libraries(${TARGET_NAME} PRIVATE Shared)

    if(WIN32)
        set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_WINDOWS)
    endif()

    if(APPLE)
        target_link_libraries(${TARGET_NAME} PRIVATE MoltenVK)
        set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
        set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST 
            ${PROJECT_SOURCE_DIR}/Resources/Platforms/Mac/Info.plist.in)

        target_sources(${TARGET_NAME} PRIVATE
            "${PROJECT_SOURCE_DIR}/Resources/Platforms/Mac/MoltenVK_icd.json")
        set_source_files_properties("${PROJECT_SOURCE_DIR}/Resources/Platforms/Mac/MoltenVK_icd.json" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/vulkan/icd.d"
        )

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${MOLTENVK_DYLIB}"
                "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/libMoltenVK.dylib"
            COMMENT "Copying MoltenVK dylib to app bundle"
        )

        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_MACOS)

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "#!/bin/bash" > "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper"
            # Disable Metal Heap to extend support for older hardware
            COMMAND ${CMAKE_COMMAND} -E echo "export MVK_CONFIG_USE_MTLHEAP=0" >> "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper"
            # Specify icd file for loader
            COMMAND ${CMAKE_COMMAND} -E echo "export VK_ICD_FILENAMES=\"$(dirname \"\$0\")/../Resources/vulkan/icd.d/MoltenVK_icd.json\"" >> "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper"
            # Launch real executable
            COMMAND ${CMAKE_COMMAND} -E echo "exec \"$(dirname \"$0\")/${TARGET_NAME}\" \"\$@\"" >> "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper"
            COMMAND chmod +x "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper"
            COMMENT "Creating launch wrapper for macOS bundle"
        )
    endif()

    if(UNIX AND NOT APPLE) 
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_LINUX)
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endfunction()