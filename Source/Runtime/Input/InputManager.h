#pragma once

#include <Platform/Abstract/Input/RawInputDevice.h>

#include "InputView.h"

#include "InputActionRecord.h"

class InputManager
{
    RawInputDevice* m_dev{};

    InputView m_view{};
    const InputView m_emptyView{};  
public:

    void Initialize() {}

    void SetInputDevice(RawInputDevice *inputDev) {m_dev = inputDev;}

    const InputView& GetView() {
        if(!m_dev) return m_emptyView;

        // Setup view

        return m_view;
    }

};