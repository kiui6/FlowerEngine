#pragma once

#include <vector>
#include <cstdint>
#include <string>

std::vector<uint8_t> CompileHLSLToDXBC(const std::string& hlslSource);
std::vector<uint8_t> CompileHLSLToDXIL(const std::string& hlslSource);