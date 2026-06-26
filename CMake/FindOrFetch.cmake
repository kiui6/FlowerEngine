function(FindOrFetchPackage PACKAGE_NAME SOURCE_LINK SOURCE_VERSION)
    message(STATUS "Searching for ${PACKAGE_NAME}")
    find_package(${PACKAGE_NAME})
    
    if(NOT ${PACKAGE_NAME}_Found)
        message(STATUS "Fetching ${PACKAGE_NAME}")
        include(FetchContent)

        FetchContent_Declare(
            ${PACKAGE_NAME}
            GIT_REPOSITORY ${SOURCE_LINK}
            GIT_TAG ${SOURCE_VERSION}
        )

        FetchContent_MakeAvailable(${PACKAGE_NAME})
    else()
        message(STATUS "Found ${PACKAGE_NAME}")
    endif()
endfunction()

function(FindOrFetchLibrary EXPORT_NAME PACKAGE_NAME SOURCE_LINK SOURCE_VERSION)
    message(STATUS "Searching for ${PACKAGE_NAME}")
    find_package(${PACKAGE_NAME})
    
    if(NOT ${PACKAGE_NAME}_Found)

    else()
        message(STATUS "Found ${PACKAGE_NAME}")
    endif()
endfunction()