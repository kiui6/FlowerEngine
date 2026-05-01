#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/BoolField.h>
#include <Record/FieldContainers/IntField.h>
#include <Record/FieldContainers/TextField.h>

class ReferenceRecord : public Record
{
public:
    Field<UInt32Field> BaseTypename = {FIELDID(NAME)};
    Field<RecordField> Base = {FIELDID(BASE)};
    Field<BoolField> IsDynamic = {FIELDID(DYNA)};
    Field<RecordField> ParentChunk = {FIELDID(CHNK)};
    Field<TextField> DisplayName = {FIELDID(DISP)};

    static uint32_t StaticType() {return CompMakeRecordType("REFR");}

    virtual std::vector<FieldBase*> GetFields() override {
        std::vector<FieldBase*> fields = Record::GetFields();
        fields.push_back(&BaseTypename);
        fields.push_back(&Base);
        fields.push_back(&IsDynamic);
        fields.push_back(&ParentChunk);
        return fields;
    }
};