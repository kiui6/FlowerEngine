#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <span>

enum class FileAccess : uint8_t {
    Read    = 1 << 0,
    Write   = 1 << 1,
    Binary  = 1 << 2,
    Append  = 1 << 3,
};

inline FileAccess operator|(FileAccess a, FileAccess b) {
    return static_cast<FileAccess>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline bool operator&(FileAccess a, FileAccess b) {
    return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

inline bool operator!(FileAccess a) {
    return static_cast<uint8_t>(a) == 0;
}

class FileBase {
public:
    virtual ~FileBase() = default;

    virtual void Open(std::string path, FileAccess access) = 0;
    virtual bool IsOpen() const = 0;
    virtual void Close() = 0;

    virtual size_t GetSize() const = 0;
    virtual const std::byte* GetData() const = 0;

    virtual size_t WriteBytes(std::span<const std::byte> buffer, size_t offset = 0) = 0;
    virtual size_t ReadBytes(std::span<std::byte> &buffer, size_t offset = 0) const = 0;

    virtual std::byte ReadByte(size_t pos) const = 0;

    virtual size_t Resize(size_t size) = 0;
};