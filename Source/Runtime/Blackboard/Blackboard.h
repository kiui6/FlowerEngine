#pragma once

#include <string>
#include <vector>

#include "BlackboardType.h"

struct BlackboardPair
{
    std::string key;
    BlackboardValueType valueType;
    BlackboardValue value;
};

class Blackboard
{
    std::vector<BlackboardPair> blackboardPairs;
public:
    int AddValue(std::string key, BlackboardValueType type, BlackboardValue value);
    BlackboardValue GetValue(std::string key);
    void RemoveValue(std::string key);
};