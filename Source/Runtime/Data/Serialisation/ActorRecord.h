#pragma once

#include <cstdint>

struct ActorRecord {
    uint64_t refID;
    uint32_t typeID;
    uint64_t world;
    uint32_t chunkX, chunkY;
    float actorX, actorY;
    uint32_t fieldCount;
};