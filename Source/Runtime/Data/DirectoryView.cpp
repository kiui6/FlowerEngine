#include "DirectoryView.h"

#include <Log/Log.h>

std::vector<DirectoryView> DirectoryView::GetEntries() const
{
    std::vector<DirectoryView> files;

    std::error_code error;
    for (auto& entry : std::filesystem::recursive_directory_iterator(m_path, error)) {
        std::filesystem::path path = std::filesystem::weakly_canonical(m_base / entry.path(), error);
        if(error) {
            LOG(Assert, LogData, "Path canonicalization failed!");
            break;
        }

        std::filesystem::path relativePath = path.lexically_relative(m_base);

        if(relativePath.empty() || *relativePath.begin() == "..") {
            LOGF(Assert, LogData, "Illegal file path: \"%s\"! Path is outside of sandbox.", relativePath.c_str());
            break;
        }

        files.push_back(DirectoryView(m_base, m_basePrefix, path));
    }

    return files; 
}

const std::string DirectoryView::GetPath() const
{
    std::filesystem::path relativePath = m_path.lexically_relative(m_base);

    if(relativePath.empty() || *relativePath.begin() == "..") {
        LOGF(Assert, LogDirectoryView, "Illegal file path: \"%s\"! Path is outside of base.", relativePath.c_str());
        return {};
    }

    return m_basePrefix + relativePath.string();
}

bool DirectoryView::HasExtension(std::string_view ext) const
{
    return m_path.extension() == ext;
}
