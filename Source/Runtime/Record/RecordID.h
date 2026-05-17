#pragma once

#include <cstdint>

using RecordID = uint64_t;

constexpr RecordID INVALID_RECORD = 0;
constexpr RecordID ENTRY_RECORD = 1;
constexpr RecordID PLAYER_RECORD = 2;
constexpr RecordID GBLACKBOARD_RECORD = 3;