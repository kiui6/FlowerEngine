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

    set(SrcRecords "Records/Test_OffsetsParsing.cpp" )

    add_executable(EngineTests "main.cpp" ${SrcRecords})

    target_link_libraries(EngineTests PRIVATE doctest)
    target_compile_definitions(EngineTests PRIVATE ENGINE_TESTS)

    include(CTest)
    add_test(NAME EngineTests COMMAND EngineTests)
endif()