#pragma once

#include <Utility/Arena.h>
#include <Utility/Memory.h>

class ScriptVMAllocator
{
    DynamicArena<Kilobytes(512)> m_arena;
};