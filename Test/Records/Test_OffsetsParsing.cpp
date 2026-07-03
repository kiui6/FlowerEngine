#include <Record/Record.h>
#include <Data/DataManager.h>

#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest/doctest.h>

TEST_CASE("DataWriter: In-Memory Read/Write") {
    DataWriter writer;
    REQUIRE(writer.Data() == nullptr);
    REQUIRE(writer.Size() == 0);
    
    SUBCASE("Write single 64bit integer") {
        writer.Write<uint64_t>(1024);
        
        REQUIRE(writer.Data() != nullptr);
        REQUIRE(writer.Size() == sizeof(uint64_t));
        REQUIRE(*reinterpret_cast<uint64_t*>(writer.Data()) == 1024);
    }

    SUBCASE("Clear after write") {
        writer.Clear();
        
        REQUIRE(writer.Data() == nullptr);
        REQUIRE(writer.Size() == 0);
    }


}