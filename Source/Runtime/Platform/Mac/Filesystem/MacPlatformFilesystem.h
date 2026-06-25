#pragma once

#include <Platform/Abstract/Filesystem/PlatformFilesystem.h>
// TODO: File implementation must be platform specific
#include <Platform/Abstract/Filesystem/File.h>

#include "MacMappedFile.h"

class MacPlatformFilesystem : public PlatformFilesystem {
public:

    // TODO: Open File should have platform-specific implementation
    virtual std::shared_ptr<File> OpenFile(const std::string& path, FileAccess access) override {return std::make_shared<File>(path, access);}
    virtual std::shared_ptr<MappedFile> MapFile(const std::string& path, FileAccess access) override {return std::shared_ptr<MacMappedFile>(new MacMappedFile(path, access));}
};