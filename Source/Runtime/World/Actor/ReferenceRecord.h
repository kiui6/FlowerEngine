#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/BoolField.h>
#include <Record/FieldContainers/IntField.h>
#include <Record/FieldContainers/TextField.h>
#include <Record/FieldContainers/StructField.h>

#include <Math/Transform.h>

class ReferenceRecord : public Record
{
    struct Transform {
        uint8_t Depth;
        int32_t LocationX, LocationY;
        int32_t PivotX, PivotY;
        float ScaleX, ScaleY;
        float Rotation;
    };
public:
    ReferenceRecord() {SetType(ReferenceRecord::StaticType());}

    Field<StructField<ReferenceRecord::Transform>> Transform = {FIELDID(TRAN)};
    Field<UInt32Field> BaseTypename = {FIELDID(NAME)};
    Field<RecordField> Base = {FIELDID(BASE)};
    Field<BoolField> IsDynamic = {FIELDID(DYNA)};
    Field<RecordField> ParentChunk = {FIELDID(CHNK)};
    Field<TextField> DisplayName = {FIELDID(DISP)};

    static ID32 StaticType() {return MakeID32("REFR");}

    virtual std::vector<FieldBase*> GetFields() override {
        std::vector<FieldBase*> fields = Record::GetFields();
        fields.push_back(&Transform);
        fields.push_back(&BaseTypename);
        fields.push_back(&Base);
        fields.push_back(&IsDynamic);
        fields.push_back(&ParentChunk);
        return fields;
    }
};