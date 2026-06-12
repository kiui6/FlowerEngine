if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "LTO Enabled")
    
    cmake_policy(SET CMP0069 NEW)
    
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Using ThinLTO")
        add_compile_options(-flto=thin)
        set(CMAKE_LINKER_TYPE LLD)
    endif()
else()
    message(STATUS "LTO Disabled")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()