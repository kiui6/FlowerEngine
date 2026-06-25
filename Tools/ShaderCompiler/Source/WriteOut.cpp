#include "WriteOut.h"

void WriteBinaryFile(const std::string &path, const std::vector<uint8_t> &data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot write to " + path);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}