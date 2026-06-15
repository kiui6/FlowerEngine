#pragma once

#include <Utility/ID.h>
#include <Record/RecordPtr.h>

#include <memory>
#include <array>
#include <concepts>

#include "Actor.h"
#include "ReferenceRecord.h"

struct ActorFactory
{
    virtual constexpr ID32 GetRecordType() {return MakeID32("UNKN");}
    std::unique_ptr<Actor> CreateActor(const RecordPtr<ReferenceRecord> & reference) {
        if(reference.IsBound()) {
            return this->CreateActorImpl(reference);
        } else {
            return nullptr;
        }
    }
protected:
    virtual std::unique_ptr<Actor> CreateActorImpl(const RecordPtr<ReferenceRecord> & reference) {return nullptr;}
};