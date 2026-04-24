#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#pragma pack(push, 1)
struct WorldRecordLUTEntry
{
    uint64_t refID;
    float x, y, x2, y2;
    uint8_t flags;
    uint64_t offset;
};

struct WorldRecordHeader
{
    uint8_t worldNameLength;
    unsigned char *worldName;

    uint64_t actorsOffset = 0;
    uint64_t actorsLutOffset = 0;
    uint64_t actorsLutCount = 0;
};
#pragma pack(pop)

class WorldRecord
{
    WorldRecordHeader m_header;
public:

};