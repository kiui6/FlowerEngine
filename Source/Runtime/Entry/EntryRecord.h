#pragma once

#include <Record/RecordFactoryLibrary.h>

#include <Record/Record.h>
#include <Record/FieldContainers/StringField.h>
#include <Record/FieldContainers/TrivialField.h>

class EntryRecord : public Record
{
public:
    EntryRecord() {SetType(EntryRecord::StaticType());}

    Field<FString> StartupScriptPath {FIELDID(SCRI)};

    static ID32 StaticType() {return MakeID32("ENTR");}

    virtual std::vector<FieldBase*> GetFields(size_t reserveSize = 2) override {
        std::vector<FieldBase*> fields = Record::GetFields(reserveSize);
        fields.push_back(&StartupScriptPath);
        return fields;
    }
};

class EntryRecordFactory : public TemplatedRecordFactory<EntryRecord>
{
    static bool sIsInitialized;
public:
};