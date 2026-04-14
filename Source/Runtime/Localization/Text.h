#pragma once

#include <string_view>
#include <string>
#include <future>

class Text
{
    std::string_view m_id;
    std::string m_cached;
public:
    Text(std::string_view id);

    std::string& Get();
    std::promise<std::string&> GetLazily();

    std::string& Format(std::string_view &arg, ...);
};