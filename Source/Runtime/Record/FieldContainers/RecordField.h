#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

class RecordField : public FieldContainerBase
{
    RecordPtr<> value;
public:
    RecordField() {}
    explicit RecordField(int) {}
    RecordField(RecordID init) : value(init) {}
};

class WeakRecordField : public FieldContainerBase
{
    WeakRecordPtr<> value;
public:
    WeakRecordField() {}
    explicit WeakRecordField(int) {}
    WeakRecordField(RecordID init) : value(init) {}
};