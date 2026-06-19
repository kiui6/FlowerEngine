#pragma once

#include <cstdint>

using RecordID = uint64_t;

static_assert(std::is_same_v<RecordID, uint64_t>);
static_assert(std::is_integral_v<RecordID>);
static_assert(sizeof(RecordID) == 8);

constexpr RecordID INVALID_RECORD = 0;
constexpr RecordID ENTRY_RECORD = 1;
constexpr RecordID PLAYER_RECORD = 2;
constexpr RecordID GBLACKBOARD_RECORD = 3;