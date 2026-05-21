#pragma once

#include <Record/RecordID.h>

struct SerialLUTEntry
{
    RecordID id;
    uint64_t offset;
    uint8_t deleted;
};