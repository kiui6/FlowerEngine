#pragma once

#include "AtlasRecord.h"
#include <Record/RecordFactoryLibrary.h>

#include <GameFramework/Actors/SpriteActor/SpriteActor.h>

class AtlasRecordFactory : public TemplatedRecordFactory<AtlasRecord>
{
    static bool sIsInitialized;
public:
    virtual Actor* CreateActor(Record* reference) override {return new SpriteActor;}
};