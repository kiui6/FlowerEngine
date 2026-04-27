#pragma once

#include "ReferenceCounter.h"

class ReferenceCounterPtr {
protected:
    void AddRef(ReferenceCounter* counter) {
        counter->AddRef();
    }
    void ReleaseRef(ReferenceCounter* counter) {
        counter->ReleaseRef();
    }
public:
    virtual ~ReferenceCounterPtr(){}
};