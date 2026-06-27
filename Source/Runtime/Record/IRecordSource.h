#pragma once

#include <optional>

#include <Utility/ID.h>

#include "RecordID.h"
#include "RecordIR/RecordObject.h"

struct RecordInformation {
    bool isDeleted = false;
    bool isDelta = false;

    uint32_t type = 0;
    uint16_t fieldsCount = 0;
};

class IRecordSource {
public:
    /*
     * Resolves unique plugin ID to a plugin-relative(internal) plugin ID
     */
    virtual bool ResolveUniquePluginID(uint64_t uniqueID, uint16_t& relativeID) = 0;
    virtual uint64_t GetUniquePluginID() = 0;

    /*
     * Fetches Record object from file and inserts it into the result
     *
     * @warning
     * This function expects RecordObject& result to be empty before any operations to allow optimization.
     * If it's not cleared before execution, behavior is undefined. 
     */
    virtual bool FetchRecordObject(RecordID id, RecordObject& result) = 0;
    /*
     * Fetches Record's LUT entry from a file
     *
     * @param result
     * RecordInformation object that will contain record information if successful 
     * 
     * @return
     * True if it record exists in the LUT table, false otherwise
     */
    virtual bool FetchRecordInfo(RecordID id, RecordInformation& result) = 0;
    virtual std::optional<ID32> FetchRecordType(RecordID id) = 0;
    virtual bool HasRecord(RecordID id) = 0;
};