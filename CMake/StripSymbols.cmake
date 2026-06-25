if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Symbols will be stripped")

    if(NOT WIN32 AND NOT APPLE)
        add_link_options(-Wl,--strip-all)
    endif()
endif()