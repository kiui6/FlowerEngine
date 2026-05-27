#pragma once

#include <Debug/DebugWindow.h>

#include <imgui/imgui.h>

class EditorWindow : public DebugWindow {
protected:
    virtual void OnUpdate() override {
        
    }

    virtual void OnRender() override {
        ImGui::ShowDemoWindow();
    }
};