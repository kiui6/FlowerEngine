#pragma once

#include <string_view>
#include <string>
#include <future>

class Text
{
    std::string_view m_id;
    std::string_view m_cached;
public:
    Text(std::string_view id);

    std::string_view& Get();
    std::promise<std::string_view&> GetLazily();

    std::string Format(std::string_view &arg, ...);
};