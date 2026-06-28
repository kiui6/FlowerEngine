#pragma once

#include <cstdint>

using RecordID = uint64_t;
static_assert(std::is_same_v<RecordID, uint64_t>);
static_assert(std::is_integral_v<RecordID>);
static_assert(sizeof(RecordID) == 8);

using PluginID = uint64_t;
static_assert(std::is_same_v<PluginID, uint64_t>);
static_assert(std::is_integral_v<PluginID>);
static_assert(sizeof(PluginID) == 8);

using PluginOrderID = uint16_t;
static_assert(std::is_same_v<PluginOrderID, uint16_t>);
static_assert(std::is_integral_v<PluginOrderID>);
static_assert(sizeof(PluginOrderID) == 2);

constexpr static RecordID INVALID_RECORD = 0;
constexpr static RecordID ENTRY_RECORD = 1;
constexpr static RecordID PLAYER_RECORD = 2;
constexpr static RecordID GBLACKBOARD_RECORD = 3;

constexpr static PluginOrderID PLUGIN_ORDER_ID_BASE = 0x0000;
constexpr static PluginOrderID PLUGIN_ORDER_ID_RUNTIME = 0xFFFF;

constexpr static uint8_t RECORD_PLUGIN_ID_BITS = 16;
constexpr static uint8_t RECORD_PLUGIN_ID_SHIFT = 48;
constexpr static uint64_t RECORD_PLUGIN_ID_MASK = ~(0xFFFFULL << RECORD_PLUGIN_ID_SHIFT);

constexpr static PluginOrderID ExtractPluginIDFromRecordID(RecordID recordID) {
    return static_cast<PluginOrderID>(recordID >> RECORD_PLUGIN_ID_SHIFT);
}

constexpr static RecordID RemovePluginIDFromRecordID(RecordID recordID) {
    return (recordID & RECORD_PLUGIN_ID_MASK);
}

constexpr static RecordID OverwritePluginIDInRecordID(RecordID recordID, PluginOrderID pluginID) {
    return (recordID & RECORD_PLUGIN_ID_MASK) | (static_cast<uint64_t>(pluginID) << RECORD_PLUGIN_ID_SHIFT);
}