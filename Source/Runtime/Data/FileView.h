#pragma once

#include <memory>
#include <cassert>

#include <Data/DataView.h>

#include <Platform/Abstract/Filesystem/FileBase.h>

class FileView : public DataView {
    friend class DataManager;
    std::shared_ptr<FileBase> m_file = nullptr;

    FileView(std::shared_ptr<FileBase> file) : m_file(file), DataView(file ? file->GetData() : nullptr, file ? file->GetSize() : 0) {}
    FileView(std::shared_ptr<FileBase> file, size_t offset) : m_file(file), DataView(file ? file->GetData() : nullptr, file ? file->GetSize() : 0, offset) {}
    FileView(std::shared_ptr<FileBase> file, size_t offset, size_t size) : m_file(file), DataView(file ? file->GetData() : nullptr, size, offset) {}
public:
    FileView() = default;

    FileView MakeSubFileView(size_t offset, size_t size) const noexcept {
        return FileView(m_file, m_offset + offset, size);
    } 
};