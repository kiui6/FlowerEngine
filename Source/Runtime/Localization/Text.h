#pragma once

#include <string_view>
#include <string>
#include <future>

class Text
{
    std::string_view m_id;
    std::string m_cached;
public:
    Text() {}
    Text(std::string_view id) {m_id = id;}

    std::string_view Get();
    std::promise<std::string_view> GetLazily();

    std::string Format(std::string_view &arg, ...);
};