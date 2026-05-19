#include "PathBuilder.h"

#include <filesystem>

std::string PathBuilder::MakeRelative(std::string_view base, std::string_view path)
{
    return std::filesystem::relative(path, base).string();
}