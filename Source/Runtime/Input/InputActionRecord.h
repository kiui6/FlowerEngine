#pragma once

#include <Record/RecordFactoryLibrary.h>
#include <Record/RecordFactory.h>

#include <Record/Record.h>
#include <Record/FieldContainers/StringField.h>
#include <Record/FieldContainers/TrivialField.h>
#include <Record/FieldContainers/ListField.h>
#include <Record/FieldContainers/MapField.h>
#include <Record/FieldContainers/PairField.h>

#include <Platform/Abstract/Input/InputAction.h>

class InputActionRecord : public Record
{
    using FInputAction = FTrivial<InputKey>;
    using FInputAxis = FPair<FTrivial<InputAxis>, FFloat>;
public:
    InputActionRecord() {SetType(InputActionRecord::StaticType());}

    static ID32 StaticType() {return MakeID32("INAC");}

    Field<FMap<FString, FList<FInputAction>>> m_actions{FIELDID(ACTS)};
    Field<FMap<FString, FList<FInputAxis>>> m_axis{FIELDID(AXIS)};

    virtual std::vector<FieldBase*> GetFields(size_t reserveSize = 3) override {
        std::vector<FieldBase*> fields = Record::GetFields(reserveSize);
        fields.push_back(&m_actions);
        fields.push_back(&m_axis);
        return fields;
    }
};

class InputActionRecordFactory : public TemplatedRecordFactory<InputActionRecord>
{
    static bool sIsInitialized;
public:
};