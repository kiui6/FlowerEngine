#pragma once

#include <string>
#include <vector>

class FileBase
{
protected:
    std::string m_path;
    bool m_open = false;
    bool m_isDirectory = false;
public:
    virtual void Open() {}
    virtual bool IsOpen() const {return m_open;}
    virtual void Close() {}

    virtual bool IsDirectory() const {return m_isDirectory;}
};