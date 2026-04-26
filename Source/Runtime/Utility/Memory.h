#pragma once

#include <cstdint>
#include <memory>

constexpr size_t Kilobytes(size_t value) {return value * 1024;}
constexpr size_t Megabytes(size_t value) {return value * 1024 * 1024;}
constexpr size_t Gigabytes(size_t value) {return value * 1024 * 1024 * 1024;}