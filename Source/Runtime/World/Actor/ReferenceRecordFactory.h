#pragma once

#include "ReferenceRecord.h"
#include <Record/RecordFactoryLibrary.h>

class ReferenceRecordFactory : public TemplatedRecordFactory<ReferenceRecord>
{
    static bool sIsInitialized;
};