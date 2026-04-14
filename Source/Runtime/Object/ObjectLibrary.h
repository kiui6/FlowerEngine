#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>
#include "Object.h"

#include <memory>
#include <atomic>

struct ObjectDescriptor
{
    uint64_t id = 0;
    std::unique_ptr<Object> object = nullptr;
    std::atomic<uint32_t> references = 0;
};

class ObjectLibrary : public IService
{
    static bool bIsInitialized;

    std::unordered_map<uint64_t, ObjectDescriptor> m_objects;
public:
    static std::string_view GetStaticName() {return "ObjectLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    // TODO: Create ObjectPtr class
    Object* LoadObject(uint64_t objectID);
    Object* GetObject(uint64_t objectID);
    bool IsValidObject(uint64_t objectID);
    
    // Marks object as garbage, will cause deletion on the next GC pass
    void MarkAsGarbage(uint64_t objectID);
};