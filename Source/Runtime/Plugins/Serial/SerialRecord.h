#pragma once

#include <Utility/ID.h>

#include "SerialField.h"

struct SerialRecord
{
    ID32 type = 0;
    uint32_t fieldCount = 0;
    SerialField* fields;
};