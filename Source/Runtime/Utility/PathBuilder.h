#pragma once

#include <string>

struct PathBuilder {
    static std::string Join(std::string_view a, std::string_view b);
    static std::string MakeAbsolute(std::string_view base, std::string_view path) { return ""; }
};