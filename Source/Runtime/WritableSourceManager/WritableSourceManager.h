#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include <Data/DataManager.h>

#include <SourceFormat/SourceReader.h>
#include <SourceFormat/SourceWriter.h>

#include <Async/AsyncService.h>

class WritableSourceManager : public IService
{
    static bool bIsInitialized;

    struct SourceHandle {
        FileView file;
        SourceReader reader;
    };

    std::optional<SourceHandle> m_context;
public:
    static std::string_view GetStaticName() {return "WritableSourceManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    bool MountSource(std::string_view path, bool unmountExisting = false);
    void UnmountSource(bool save = false);

    bool SaveSource();
};