#pragma once

#include <cstdint>
#include <vector>

#include <Platform/MappedFile.h>

#include "../../Serialisation/ObjectRecord.h"
#include "../../Serialisation/ObjectField.h"
#include "../LUT.h"

#pragma pack(push, 1)
struct MasterFileHeader
{
    char signature[4] = {'M', 'S', 'T', 'R'};
    uint16_t versionMajor = 0;
    uint16_t versionMinor = 0;

    uint64_t pluginID = 0;
    uint64_t pluginVersion = 0;

    uint64_t recordsOffset = 0;
    uint64_t recordsLutOffset = 0;
    uint64_t recordsLutCount = 0;
    
    uint64_t worldsOffset = 0;
    uint64_t worldsLutOffset = 0;
    uint64_t worldsLutCount = 0;

    uint64_t blackboardOffset = 0;
    uint64_t blackboardLutOffset = 0;
    uint64_t blackboardLutCount = 0;
};
#pragma pack(pop)

static_assert(sizeof(MasterFileHeader) == 96 && "Unexpected master file header size");

class MasterFile
{
    MasterFileHeader m_header;
    std::unique_ptr<MappedFile> m_file;
public:
    
};