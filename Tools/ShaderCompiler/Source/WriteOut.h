#pragma once

#include <fstream>
#include <vector>
#include <cstdint>

void WriteBinaryFile(const std::string& path, const std::vector<uint8_t>& data);
void WriteCArrayFile(const std::string& path, const std::vector<uint8_t>& data);