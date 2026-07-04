cmake_minimum_required(VERSION 3.21)

option(ENABLE_ENGINE_TESTS "Enable engine unit tests" OFF)

if(ENABLE_ENGINE_TESTS)
    message(STATUS "Tests enabled")

    message(STATUS "Fetching doctest")
    include(FetchContent)
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
    )
    FetchContent_MakeAvailable(doctest)

    add_executable(EngineTests "main.cpp" "Records/Test_DataWriter.cpp")

    target_link_libraries(EngineTests PRIVATE SharedModules doctest SDL3::SDL3-static)
    target_compile_definitions(EngineTests PRIVATE ENGINE_TESTS)

    if(WIN32)
        target_compile_definitions(EngineTests PRIVATE PLATFORM_WINDOWS)
    endif()
        
    if(APPLE)
        target_compile_definitions(EngineTests PRIVATE PLATFORM_MAC)
    endif()

    if(UNIX AND NOT APPLE) 
        target_compile_definitions(EngineTests PRIVATE PLATFORM_LINUX)
    endif()

    include(CTest)
    add_test(NAME EngineTests COMMAND EngineTests)
endif()