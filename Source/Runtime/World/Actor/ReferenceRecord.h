#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/TextField.h>
#include <Record/FieldContainers/TrivialField.h>
#include <Record/FieldContainers/TupleField.h>

#include <Math/Transform.h>

class ReferenceRecord : public Record
{
    struct TransformTuple : public TupleField<int32_t, int32_t, int32_t, int32_t, float, float, float, uint8_t> {
        int32_t& LocationX() {return get<0>();}
        int32_t& LocationY() {return get<1>();}
        int32_t& PivotX() {return get<2>();}
        int32_t& PivotY() {return get<3>();}
        float& ScaleX() {return get<4>();}
        float& ScaleY() {return get<5>();}
        float& Rotation() {return get<6>();}
        uint8_t& Depth() {return get<7>();}
    };
public:
    ReferenceRecord() {SetType(ReferenceRecord::StaticType());}

    Field<FTuple<ReferenceRecord::TransformTuple>> Transform = {FIELDID(TRAN)};
    Field<FTrivial<uint32_t>> BaseTypename = {FIELDID(NAME)};
    Field<FRecord> Base = {FIELDID(BASE)};
    Field<FTrivial<bool>> IsDynamic = {FIELDID(DYNA)};
    Field<FRecord> ParentChunk = {FIELDID(CHNK)};
    Field<FText> DisplayName = {FIELDID(DISP)};

    static ID32 StaticType() {return MakeID32("REFR");}
};