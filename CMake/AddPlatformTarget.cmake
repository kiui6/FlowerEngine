function(add_platform_target TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})

    set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

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

        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_MAC)

        set(MAC_FLOWER_WRAPPER_PATH "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/MacOS/FlowerWrapper.sh")
        configure_file(
            ${PROJECT_SOURCE_DIR}/Resources/Platforms/Mac/FlowerWrapper.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/FlowerWrapper.sh
            @ONLY
        )

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/FlowerWrapper.sh
                ${MAC_FLOWER_WRAPPER_PATH}
            COMMAND chmod +x ${MAC_FLOWER_WRAPPER_PATH}
            COMMENT "Installing launch wrapper for macOS bundle"
        )
    endif()

    if(UNIX AND NOT APPLE) 
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_LINUX)
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endfunction()