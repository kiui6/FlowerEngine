#pragma once

#include <Utility/Arena.h>
#include <Utility/Memory.h>

class ScriptVMAllocator
{
    GrowingArena<Kilobytes(512)> m_arena;
};