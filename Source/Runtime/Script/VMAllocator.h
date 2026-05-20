#pragma once

#include <Utility/Arena.h>
#include <Utility/Memory.h>

class ScriptVMAllocator
{
    ArenaAllocator<Kilobytes(512)> m_arena;
};