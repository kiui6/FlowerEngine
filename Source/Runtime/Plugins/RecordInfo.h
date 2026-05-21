#pragma once

#include <vector>

#include <Utility/ID.h>
#include <Record/RecordID.h>

struct FieldInfo {
    ID32 id = 0;
    std::vector<std::byte> data;
};

struct RecordInfo {
    uint64_t pluginID;

    ID32 type = 0;
    RecordID id = 0;
    std::vector<FieldInfo> fields;
};