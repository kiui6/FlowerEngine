#pragma once

#include <vector>
#include <string>
#include <optional>
#include <string_view>
#include <unordered_map>

#include "Hash.h"

class ArgParser {
    std::unordered_map<std::string, std::optional<std::string>, StringHash, std::equal_to<>> m_arguments;
public:
    ArgParser(int argc, char ** argv) {
        bool bExpectValue = false;
        std::string_view complimentKey;
        for(int i = 0; i < argc; i++) {
            std::string_view arg = argv[i];

            if(!bExpectValue) {
                if(!arg.starts_with("--")) {
                    if(arg.starts_with("-")){
                        arg.remove_prefix(1);
                        m_arguments.emplace(arg, std::optional<std::string>{});  
                    }
                } else {
                    arg.remove_prefix(2);
                    complimentKey = arg;
                    bExpectValue = true;
                }
            } else {
                m_arguments.emplace(complimentKey, arg);
                bExpectValue = false;
            }
        }
    }

    size_t GetCount() const {return m_arguments.size();}

    bool Has(std::string_view argument) const {return m_arguments.find(argument) != m_arguments.end();}
    std::optional<std::string_view> Get(std::string_view argument) const {
        auto it = m_arguments.find(argument);
        if(it == m_arguments.end()) {
            return {};
        }
        return *it->second;
    }
};