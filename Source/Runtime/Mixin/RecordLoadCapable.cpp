#include "RecordLoadCapable.h"

#include <Record/Record.h>
#include <Record/RecordLibrary.h>

Record *RecordLoadCapable::LoadRecord(RecordID id)
{
    return GetService<RecordLibrary>()->LoadRecordRaw(id);
}