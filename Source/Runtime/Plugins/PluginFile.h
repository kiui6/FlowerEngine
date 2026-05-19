#pragma once

#include <Record/IRecordSource.h>
#include <DataManager/DataView.h>
#include <DataManager/DataReader.h>

class PluginFile : public IRecordSource {
    DataView m_fileView;
    DataView m_recordsView;
    DataView m_LUTView;
    bool m_isMaster;
public:
    PluginFile();

    RecordMemory FetchRecordMemory(RecordID id) override {return {};}
    bool HasRecord(RecordID id) override {return false;}
};