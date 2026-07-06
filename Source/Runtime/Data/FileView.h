#pragma once

#include <memory>
#include <cassert>

#include <Data/DataView.h>

#include <Platform/Abstract/Filesystem/FileBase.h>

class FileView {
    std::shared_ptr<FileBase> m_file = nullptr;
public:
    FileView() = default;
    FileView(const std::shared_ptr<FileBase>& file) : m_file(file) {}
    FileView(std::shared_ptr<FileBase> && file) : m_file(std::move(file)) {}

    DataView MakeView(size_t offset = 0) {
        if(!m_file) {
            return DataView();
        }
        return DataView(m_file->GetData(), m_file->GetSize(), offset);
    } 

    DataView MakeView(size_t offset, size_t size) {
        if(!m_file) {
            return DataView();
        }
        return DataView(m_file->GetData(), size, offset);
    }

    operator bool() const {return m_file != nullptr;}
};