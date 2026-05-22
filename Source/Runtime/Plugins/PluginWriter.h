#pragma once

#include <vector>

#include "Serial/SerialHeader.h"
#include "Serial/SerialRecord.h"
#include "Serial/SerialDependency.h"
#include "Serial/SerialLUTEntry.h"

#include "RecordInfo.h"

class PluginReader;

class PluginWriter {
    PluginReader* m_reader = nullptr;
public:
    PluginWriter() = default;
    PluginWriter(PluginReader* reader) : m_reader(reader) {}

    void AddRecord(const RecordInfo& info);
    void AddDependency(uint64_t id);

    void RemoveRecord(RecordID id);
    void RemoveDependency(uint64_t id);
};