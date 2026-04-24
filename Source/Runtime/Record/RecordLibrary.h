#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>
#include <Mixin/ThreadAware.h>

#include "Record.h"

#include <memory>
#include <atomic>
#include <array>

struct RecordDescriptor
{
    uint64_t id = 0;
    std::unique_ptr<Record> record = nullptr;
    uint32_t type = CompMakeRecordType("UNKN");
    std::atomic<uint32_t> references = 0;
};

class RecordLibrary : public IService, protected ThreadAware
{
    static bool bIsInitialized;

    std::unordered_map<uint64_t, std::unique_ptr<Record>> m_records;
    std::vector<RecordRefCount*> m_unloadCandidates;
public:
    static std::string_view GetStaticName() {return "RecordLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    // TODO: Create RecordPtr class
    Record* LoadRecord(uint64_t recordID);
    Record* GetRecord(uint64_t recordID);
    bool IsValidRecord(uint64_t recordID);

    // Marks Record as garbage, will cause deletion on the next GC pass, unless reference is reacquired
    // Will not unload record unless refCount is actually zero when GC pass runs
    void MarkForUnload(RecordRefCount* refCount);

    /* Forces immediate synchronous GC pass
     *
     * @param bool unrestricted
     * Removes time restrictions for this GC pass
     */
    void RunGCPass(bool unrestricted = false);
};