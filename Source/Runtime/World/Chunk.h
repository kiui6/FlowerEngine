#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Actor.h"

class WorldChunk
{
protected:
    uint32_t m_chunkX = 0, m_chunkY = 0;
    float m_worldX = 0, m_worldY = 0;
    bool m_visible = false; 

    std::vector<RecordID> m_staticActors;
    std::vector<RecordID> m_dynamicActors;
public:
};