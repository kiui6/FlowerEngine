#pragma once

#include <Record/Record.h>

class ReferenceRecord : public Record
{
    Field<BoolField> f_isDynamic = {FIELDID(DYNA)};
};