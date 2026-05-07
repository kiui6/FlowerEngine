#pragma once

#include <Record/RecordID.h>

class Record;

class RecordLoadCapable {
protected:
    Record* LoadRecord(RecordID id);
};