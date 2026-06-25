#include "Blackboard.h"

#include <algorithm>

int Blackboard::AddValue(std::string key, BlackboardValueType type, BlackboardValue value)
{
    if(std::find_if(blackboardPairs.begin(), blackboardPairs.end(), [key](BlackboardPair& pair){return pair.key == key;}) == blackboardPairs.end())
    {
        return -1;
    }

    blackboardPairs.push_back({key, type, value});
    return blackboardPairs.size() - 1;
}