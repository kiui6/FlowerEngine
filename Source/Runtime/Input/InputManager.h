#pragma once

#include <Platform/Abstract/Input/RawInputDevice.h>

#include "InputView.h"

#include "InputActionRecord.h"

class InputManager
{
    RawInputDevice* m_dev{};
    RecordPtr<InputActionRecord> m_actionRec;

    InputView m_view{};
public:

    void Initialize() {}

    void SetInputDevice(RawInputDevice *inputDev) { m_dev = inputDev; m_view.m_dev = m_dev; }

    void SetInputActionRecord(const RecordPtr<InputActionRecord>& record) { m_actionRec = record; m_view.m_actionRec = m_actionRec; }
    void SetInputActionRecord(RecordPtr<InputActionRecord>&& record) { m_actionRec = record; m_view.m_actionRec = m_actionRec; }

    const InputView& GetView() {
        return m_view;
    }

};