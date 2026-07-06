#pragma once

#include <vector>

#include "Serial/SerialHeader.h"
#include "Serial/SerialRecord.h"
#include "Serial/SerialDependency.h"
#include "Serial/SerialLUTEntry.h"

#include <Record/RecordIR/RecordObject.h>

#include <Data/DataWriter.h>

class SourceWriter {
    DataWriter m_writer;

    struct RecordData {
        RecordID id;
        uint32_t type;
        uint16_t flags;
        std::vector<uint8_t> nodeData;
        std::vector<uint8_t> stringData;
    };

    std::vector<RecordData> m_recordBlobs;
public:
    SourceWriter(DataWriter && writer) : m_writer(writer) {}

    void AddRecord(uint64_t owningRecord, const RecordObject& rec);
    void AddDependency(uint64_t id);

    void RemoveRecord(RecordID id);
    void RemoveDependency(uint64_t id);
};