#include <Record/Record.h>
#include <Data/DataManager.h>

#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest/doctest.h>

TEST_CASE("DataWriter: In-Memory Read/Write") {
    DataWriter writer;
    REQUIRE(writer.Data() == nullptr);
    REQUIRE(writer.Size() == 0);

    struct alignas(8) {
            uint64_t data1;
            uint32_t data2;
            uint16_t data3;
            uint8_t data4;
            uint8_t data5;
    } alignedStruct {64, 32, 16, 81, 82};
    REQUIRE(sizeof(alignedStruct) == 16);
    
    SUBCASE("Write single 64bit integer") {
        writer.Write<uint64_t>(1024);
        
        REQUIRE(writer.Data() != nullptr);
        REQUIRE(writer.Size() == sizeof(uint64_t));
        REQUIRE(*reinterpret_cast<uint64_t*>(writer.Data()) == 1024);
    }

    SUBCASE("Write aligned struct") {
        writer.Write<decltype(alignedStruct)>(alignedStruct);
        
        REQUIRE(writer.Data() != nullptr);
        REQUIRE(writer.Size() == sizeof(alignedStruct));

        decltype(alignedStruct)& structRef = *reinterpret_cast<decltype(alignedStruct)*>(writer.Data());
        REQUIRE(structRef.data1 == 64);
        REQUIRE(structRef.data2 == 32);
        REQUIRE(structRef.data3 == 16);
        REQUIRE(structRef.data4 == 81);
        REQUIRE(structRef.data5 == 82);
    }

    SUBCASE("Add multiple, then clear") {
        writer.Write<uint64_t>(1024);
        writer.Write<bool>(false);
        writer.Write<uint32_t>(89);
        writer.Write<decltype(alignedStruct)>(alignedStruct);
        
        REQUIRE(writer.Data() != nullptr);
        REQUIRE(writer.Size() == sizeof(uint64_t) + sizeof(bool) + sizeof(uint32_t) + sizeof(alignedStruct));

        writer.Clear();
        
        REQUIRE(writer.Size() == 0);
    }


}