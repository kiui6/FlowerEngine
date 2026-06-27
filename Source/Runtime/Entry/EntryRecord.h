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
};

class EntryRecordFactory : public TemplatedRecordFactory<EntryRecord>
{
    static bool sIsInitialized;
public:
};