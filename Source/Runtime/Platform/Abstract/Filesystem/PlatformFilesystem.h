#pragma once

#include "MappedFile.h"
#include "File.h"

#include <memory>

class PlatformFilesystem
{
public:
    PlatformFilesystem() = default;
    virtual ~PlatformFilesystem() = default;

    virtual std::shared_ptr<File> OpenFile(const std::string& path, FileAccess access) = 0;
    virtual std::shared_ptr<MappedFile> MapFile(const std::string& path, FileAccess access) = 0;

    virtual std::string GetBaseDirectory();
    virtual std::string GetPreferencesDirectory(std::string_view org, std::string_view app);
};