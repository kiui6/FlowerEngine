#include "Record.h"

#include "RecordLibrary.h"
#include <Log/Log.h>

void Record::RequestSelfDestruction()
{
    RecordLibrary* library = GetService<RecordLibrary>();
    if(library) {
        library->UnloadRecord(m_id);
    } else {
        LOG(Assert, LogAsset, "Attempted to destroy Record, but RecordLibrary service doesn't exist");
    }
}