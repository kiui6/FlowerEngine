#pragma once

#include <filesystem>
#include <vector>

class DirectoryView {
    friend class DataManager;

    std::filesystem::path m_base;
    std::string m_basePrefix;
    std::filesystem::path m_path;

    DirectoryView() = default;
    DirectoryView(std::filesystem::path base, std::string basePrefix, std::filesystem::path path) 
                    : m_base(base), m_basePrefix(basePrefix), m_path(path) {}
public:
    std::vector<DirectoryView> GetEntries() const;
    const std::string GetPath() const;
    const std::string GetName() const;

    bool IsFile() const {return std::filesystem::is_regular_file(m_path);}
    bool IsDirectory() const {return std::filesystem::is_directory(m_path);}

    bool HasExtension(std::string_view ext) const;
};