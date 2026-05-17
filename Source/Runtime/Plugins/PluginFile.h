#pragma once

#include <Record/IRecordSource.h>

class PluginFile : public IRecordSource {
    bool m_isMaster;
public:

    RecordMemory FetchRecordMemory(RecordID id) override {return {};}
    bool HasRecord(RecordID id) override {return false;}
};