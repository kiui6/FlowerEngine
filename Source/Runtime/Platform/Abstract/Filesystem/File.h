#pragma once

#include "FileBase.h"

#include <SDL3/SDL.h>

// TODO: Make File class platform dependent
class File : public FileBase
{
    SDL_IOStream* m_stream = nullptr;
    size_t m_size = 0;
    std::byte* m_data = nullptr;
public:
    File() = default;
    File(const std::string& path, FileAccess access) {Open(path, access);}
    ~File();

    virtual void Open(const std::string& path, FileAccess access) override;
    virtual bool IsOpen() const override {return m_stream != nullptr;};
    virtual void Close() override;

    virtual size_t GetSize() const override {return m_size;}
    virtual const std::byte* GetData() const override {return m_data;}

    virtual size_t WriteBytes(const std::span<const std::byte>& buffer, size_t offset = 0) override;
    virtual size_t ReadBytes(std::span<std::byte> &buffer, size_t offset = 0) const override;

    virtual std::byte ReadByte(size_t pos) const override {return m_data[pos];}

    virtual size_t Resize(size_t size) override;
};