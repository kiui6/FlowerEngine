#pragma once

#include <string_view>

#include <Record/RecordPtr.h>
#include <Platform/Abstract/Input/RawInputDevice.h>

#include "InputActionRecord.h"

class InputView
{
    friend class InputManager;

    RawInputDevice* m_dev{};
    WeakRecordPtr<InputActionRecord> m_actionRec{};
public:
    bool GetAction(std::string_view actionName) const {
        if(!m_dev || !m_actionRec) {
            return false;
        }
        
        const auto& actionHashmap = m_actionRec->actions.Get();
        auto it = actionHashmap.find(actionName);
        if(it == actionHashmap.end()) {
            return false;
        }

        for(InputKey actionCode : it->second) {
            if(m_dev->GetKeyState(actionCode)) return true;
        }

        return false;
    }

    bool GetActionPressed(std::string_view actionName) {return false;}
    bool GetActionReleased(std::string_view actionName) {return false;}

    float GetAxis(std::string_view axisName) const {
        if(!m_dev || !m_actionRec) {
            return 0.f;
        }

        const auto& axisHashmap = m_actionRec->axis.Get();
        auto it = axisHashmap.find(axisName);
        if(it == axisHashmap.end()) {
            return false;
        }

        for(const std::pair<InputAxis, float>& pair : it->second) {
            float value = m_dev->GetAxisValue(pair.first);
            float multiplier = pair.second;
            if(value > 0.05f || value < 0.05f) {
                return value * multiplier;
            }
        }

        return 0.f;
    }
};