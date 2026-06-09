#pragma once

#include <string_view>

class InputView
{

public:

    bool GetAction(std::string_view actionName) const;
    float GetAxis(std::string_view axisName) const;
};