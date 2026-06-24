function(add_platform_target TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})

    target_link_libraries(${TARGET_NAME} PRIVATE Shared)

    if(WIN32)
        set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_WINDOWS)
    endif()

    if(APPLE)
        set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
        set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST 
            ${PROJECT_SOURCE_DIR}/Resources/Platforms/Mac/Info.plist.in)
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_MACOS)
    endif()

    if(UNIX AND NOT APPLE) 
        target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_LINUX)
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endfunction()